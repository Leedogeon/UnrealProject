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

