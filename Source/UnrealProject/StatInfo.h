// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatInfo.generated.h"
class UNREALPROJECT_API StatInfo
{
public:
	StatInfo();
	~StatInfo();

};
USTRUCT(BlueprintType)
struct FStat
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Lv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Money;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMP;
};