// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfo.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "PlayerAction.h"
//Widget
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

// Sets default values
APlayerInfo::APlayerInfo() : Super()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUD(TEXT(""));
	//if (PlayerHUD.Succeeded())
	//{
	//	InvenHUD = PlayerHUD.Class;
	//}

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, 270.f, 0.f));
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DM(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (DM.Succeeded())
	{
		DefaultMappingContext = DM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAJ(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Jump.IA_Jump'"));
	if (IAJ.Succeeded())
	{
		JumpAction = IAJ.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAF(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Fire.IA_Fire'"));
	if (IAF.Succeeded())
	{
		FireAction = IAF.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAD(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Dash.IA_Dash'"));
	if (IAD.Succeeded())
	{
		DashAction = IAD.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAM(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Move.IA_Move'"));
	if (IAM.Succeeded())
	{
		MoveAction = IAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAL(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Look.IA_Look'"));
	if (IAL.Succeeded())
	{
		LookAction = IAL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IALoot(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Loot.IA_Loot'"));
	if (IALoot.Succeeded())
	{
		Interaction = IALoot.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAInven(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Inven.IA_Inven'"));
	if (IAInven.Succeeded())
	{
		OpenInventory = IAInven.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));
	if (AI.Succeeded())
	{
		GetMesh()->AnimClass = AI.Class;
	}
	ForStat.Lv = 1;
	ForStat.MaxHP = 100;
	ForStat.Money = 100;
	ForStat.MaxMP = 100;
	PShouldAttack = true;
}
// Called when the game starts or when spawned
void APlayerInfo::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = Cast<UPlayerAction>(GetMesh()->GetAnimInstance());
	AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerInfo::OnAttackMontageEnded);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->ViewPitchMin = -30.0;
			PlayerController->PlayerCameraManager->ViewPitchMax = 75.0;
		}
	}
	
}
void APlayerInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector FocusSumVec;
	FVector AimFVector = GetBaseAimRotation().Vector();
	FVector PlayerVector = GetActorForwardVector();
	FocusStartVec = GetActorLocation();
	FocusSumVec.X = PlayerVector.X * FindRange;
	FocusSumVec.Y = PlayerVector.Y * FindRange;
	FocusSumVec.Z = AimFVector.Z * FindRange;
	FocusEndVec = FocusStartVec + FocusSumVec;


	//¹ë·±½º Á¶Àý
	if (DoDash)
	{
		DashGage -= DeltaTime * 1.f;
	}
	if (DashGage < 10.f)
	{
		ShouldDash = false;
	}
	if (!DoDash && DashGage < 100.f)
	{
		DashGage += DeltaTime * 70.f;
	}
	if (!ShouldDash && DashGage > 70.f)
	{
		ShouldDash = true;
	}
}
void APlayerInfo::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

	//if (IsValid(AnimInstance))
	//{
	//	PShouldAttack = true;
	//}

}
// Called to bind functionality to input
void APlayerInfo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerInfo::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APlayerInfo::StopFire);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &APlayerInfo::DashStart);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &APlayerInfo::DashEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerInfo::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerInfo::Look);

		//Looting
		EnhancedInputComponent->BindAction(Interaction, ETriggerEvent::Triggered, this, &APlayerInfo::Looting);
		//Inventory
		EnhancedInputComponent->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &APlayerInfo::OpenInven);
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed."), *GetNameSafe(this));
	}

}

void APlayerInfo::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerInfo::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		
	}
}

void APlayerInfo::Looting()
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->LootingMontage();
	}
}

void APlayerInfo::OpenInven()
{

}

void APlayerInfo::Fire()
{
	if (PShouldAttack)
	{
		if (IsValid(AnimInstance))
		{
			AnimInstance->PlayerFireMontage();
		}
	}
}

void APlayerInfo::StopFire()
{
}

void APlayerInfo::DashStart()
{
	if (ShouldDash)
	{
		DoDash = true;
		UCharacterMovementComponent* Charactermovement = GetCharacterMovement();
		Charactermovement->MaxWalkSpeed = 1500.f;
	}

	if (!ShouldDash)
	{
		DashEnd();
	}
	
	
}
void APlayerInfo::DashEnd()
{
	DoDash = false;
	UCharacterMovementComponent* Charactermovement = GetCharacterMovement();
	Charactermovement->MaxWalkSpeed = 500.f;
}


