// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"



void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UPlayerAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		Player = Cast<APlayerInfo>(Pawn);
		if (IsValid(Player))
		{
			CharacterMovement = Player->GetCharacterMovement();

		}

	}
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Player))
	{
		Velocity = CharacterMovement->Velocity;
		FRotator Rotation = Player->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity);

		Vertical = UnRotateVector.X;
		Horizontal = UnRotateVector.Y;

		Speed = Player->GetVelocity().Size2D();

		auto Acceleration = CharacterMovement->GetCurrentAcceleration();
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();

		IsFalling = CharacterMovement->IsFalling();

	}

}
