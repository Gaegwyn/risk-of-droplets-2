// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.generated.h"

/**
 * 
 */

UENUM(BlueprintType, meta = (DisplayName = "Damage Type"))
enum class EDamageType : uint8
{
	None		UMETA(DisplayName = "None"),
	Melee		UMETA(DisplayName = "Melee"),
	Projectile	UMETA(DisplayName = "Projectile"),
};

USTRUCT(BlueprintType, meta = (DisplayName = "Damage Info"))
struct RISKOFDROPLETS2_API FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	float Amount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	EDamageType DamageType = EDamageType::None;
};
