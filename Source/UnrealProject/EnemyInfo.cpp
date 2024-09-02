// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInfo.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AEnemyInfo::AEnemyInfo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux'"));
	if(SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, 270.f, 0.f));
	}


	ForStat.Lv = 1;
	ForStat.MaxHP = 100;
	ForStat.Money = 100;
}

// Called when the game starts or when spawned
void AEnemyInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyInfo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

