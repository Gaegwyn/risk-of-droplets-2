// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"

#include "Huntress.generated.h"

/**
 * 
 */
UCLASS()
class RISKOFDROPLETS2_API AHuntress : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Set default values for Huntress' properties
	AHuntress();

protected:
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Skills
	virtual void UsePrimarySkill() override;
	virtual void UseSecondarySkill() override;
	virtual void UseUtilitySkill() override;
	virtual void UseSpecialSkill() override;

	// Auto-targeting for primary and secondary skills
	void GetNearestTarget();

	// Keep track of what our target is
	TObjectPtr<AActor> CurrentTarget;

	// Skill projectile references
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AStrafeProjectile> PrimarySkillProjectile;

	// Skill projectile references
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALaserGlaiveProjectile> SecondarySkillProjectile;
	
};
