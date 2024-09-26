// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/DamageSystemComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"

#include "BaseEnemy.generated.h"

UCLASS()
class RISKOFDROPLETS2_API ABaseEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UDamageSystemComponent* GetDamageSystem() const { return DamageSystemComponent; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IDamageable functionality
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() override;
	UFUNCTION(BlueprintCallable)
	virtual float GetCurrentHealth() override;
	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;
	virtual void Heal(const float Value) override;

	// Character skills that should be overwritten in child class
	// TODO: Consider creating a separate SkillsComponent..
	virtual bool UsePrimarySkill();
	virtual bool UseSecondarySkill();
	virtual bool UseUtilitySkill();
	virtual bool UseSpecialSkill();

	// TODO: Consider tracking time remaining for UI (GetWorld()->GetTimerManager().GetTimerRemaining)
	UFUNCTION(BlueprintCallable)
	void ResetPrimarySkillCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetSecondarySkillCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetUtilitySkillCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetSpecialSkillCooldown();

	// AI
	const TObjectPtr<class UBehaviorTree> GetBehaviorTree() const;

protected:
	// Skill timers
	FTimerHandle PrimarySkillTimerHandle;
	float PrimarySkillCooldown;
	bool bCanUsePrimarySkill;

	FTimerHandle SecondarySkillTimerHandle;
	float SecondarySkillCooldown;
	bool bCanUseSecondarySkill;

	FTimerHandle UtilitySkillTimerHandle;
	float UtilitySkillCooldown;
	bool bCanUseUtilitySkill;

	FTimerHandle SpecialSkillTimerHandle;
	float SpecialSkillCooldown;
	bool bCanUseSpecialSkill;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDamageSystemComponent> DamageSystemComponent;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Speed;

};
