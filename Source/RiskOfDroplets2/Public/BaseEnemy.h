// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/DamageSystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/Damageable.h"

#include "BaseEnemy.generated.h"

UCLASS()
class RISKOFDROPLETS2_API ABaseEnemy : public APawn, public IDamageable
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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDamageSystemComponent> DamageSystemComponent;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Speed;

};
