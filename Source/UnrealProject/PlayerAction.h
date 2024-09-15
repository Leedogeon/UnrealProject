// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAnim.h"
#include "PlayerInfo.h"
#include "PlayerAction.generated.h"


/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPlayerAction : public UPlayerAnim
{
	GENERATED_BODY()

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LootMontage;
public:
	UPlayerAction();
public:
	void PlayerFireMontage();
	void LootingMontage();
	
};
