// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "DamageSystemComponent.generated.h"


// TODO: Probably rename to HealthSystemComponent ?
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISKOFDROPLETS2_API UDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSystemComponent();

	const float GetMaxHealth() const;

	const float GetCurrentHealth() const;

	void TakeDamage(const struct FDamageInfo& DamageInfo);

	void Heal(const float Value);	// TODO: Should this also take DamageInfo?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
	float CurrentHealth;

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};