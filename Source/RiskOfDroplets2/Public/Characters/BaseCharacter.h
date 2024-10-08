// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/DamageSystemComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"

#include "BaseCharacter.generated.h"

/**
 *
 */
UCLASS()
class RISKOFDROPLETS2_API ABaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UDamageSystemComponent* GetDamageSystem() const { return DamageSystemComponent; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsSprinting() const;

	// TODO: Consider tracking time remaining for UI (GetWorld()->GetTimerManager().GetTimerRemaining)
	UFUNCTION(BlueprintCallable)
	void ResetPrimarySkillCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetSecondarySkillCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetUtilitySkillCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetSpecialSkillCooldown();

private:
	// Locomotion
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Sprint();
	void StopSprinting();

	// Character skills that should be overwritten in child class
	virtual void UsePrimarySkill();
	virtual void UseSecondarySkill();
	virtual void UseUtilitySkill();
	virtual void UseSpecialSkill();

public:
	// IDamageable functionality
	virtual float GetMaxHealth() override;
	virtual float GetCurrentHealth() override;
	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;
	virtual void Heal(const float Value) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDamageSystemComponent> DamageSystemComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MovementInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> SprintInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> PrimarySkillInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> SecondarySkillInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> UtilitySkillInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> SpecialSkillInputAction;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float SprintSpeed;

	/** Specify whether we're Sprinting or not */
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	uint32 bIsSprinting:1;


	/* Box Testing */
public:
	UPROPERTY(EditAnywhere, Category="Debugging")
	float ExtentX = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	float ExtentY = 250.0f;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	float ExtentZ = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	float BoxRange = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	float CameraRotationScaler = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	FVector Offset = FVector(0, 0, 250.0f);

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
};
