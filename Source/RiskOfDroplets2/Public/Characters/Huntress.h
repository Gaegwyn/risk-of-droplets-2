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
	// TODO: Cooldowns
	virtual void UsePrimarySkill() override;
	virtual void UseSecondarySkill() override;
	virtual void UseUtilitySkill() override;
	virtual void UseSpecialSkill() override;

	// Auto-targeting for primary and secondary skills
	void GetNearestTarget();

	// Teleport a distance in the direction of the player's camera
	void StartBlink();
	void BlinkTick();
	void OnBlinkEnd();

	// Keep track of what our target is
	TObjectPtr<AActor> CurrentTarget;

	// Skill projectile references
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AStrafeProjectile> PrimarySkillProjectile;

	// Skill projectile references
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALaserGlaiveProjectile> SecondarySkillProjectile;

	// Blink Skill params
	FTimerHandle BlinkTimerHandle;

	FVector BlinkStartLocation;
	FVector BlinkTargetLocation;

	UPROPERTY(EditAnywhere)
	float BlinkDistance;
	float BlinkTimePassed;
	float BlinkDuration;	// Total time for blink to execute

	UPROPERTY(EditAnywhere)
	float AutoAimDistance;
	
};
