// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEnemy.h"

#include "Mimic.generated.h"

UCLASS()
class RISKOFDROPLETS2_API AMimic : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMimic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Skills
	virtual bool UsePrimarySkill() override;
	virtual bool UseSecondarySkill() override;
	virtual bool UseUtilitySkill() override;
	virtual bool UseSpecialSkill() override;
	
	
};
