// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAction.h"
#include "Animation/AnimMontage.h"
UPlayerAction::UPlayerAction()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Script/Engine.AnimMontage'/Game/ParagonGideon/Characters/Heroes/Gideon/Animations/Primary_Attack_A_Medium_Montage.Primary_Attack_A_Medium_Montage'"));
	if (AM.Succeeded())
	{
		FireMontage = AM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AML(TEXT("/Script/Engine.AnimMontage'/Game/ParagonGideon/Characters/Heroes/Gideon/Animations/Primary_Attack_A_Medium_Montage.Primary_Attack_A_Medium_Montage'"));
	if (AML.Succeeded())
	{
		LootMontage = AML.Object;
	}
}

void UPlayerAction::PlayerFireMontage()
{
	
	if (IsValid(FireMontage))
	{
		if (!Montage_IsPlaying(FireMontage))
		{
			Montage_Play(FireMontage);
		}
	}
}

void UPlayerAction::LootingMontage()
{
	if (IsValid(LootMontage))
	{
		if (!Montage_IsPlaying(LootMontage))
		{
			Montage_Play(LootMontage);

		}
	}
}

