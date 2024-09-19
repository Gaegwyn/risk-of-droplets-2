// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageSystemComponent.h"

#include "Types/DamageInfo.h"


// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

const float UDamageSystemComponent::GetMaxHealth() const
{
	return MaxHealth;
}

const float UDamageSystemComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UDamageSystemComponent::TakeDamage(const FDamageInfo& DamageInfo)
{
	// Apply damage dealt to character
	UE_LOG(LogTemp, Log, TEXT("Dealt Damage: %f"), DamageInfo.Amount);
	if (CurrentHealth > 0.0f)
	{
		CurrentHealth -= DamageInfo.Amount;
		UE_LOG(LogTemp, Log, TEXT("Current Health: %f"), CurrentHealth);
	}
}

void UDamageSystemComponent::Heal(const float Value)
{
	// For now just clamp it to MaxHealth. Later we may want to let it go over or something
	if (CurrentHealth > 0.0f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.0f, MaxHealth);
		UE_LOG(LogTemp, Log, TEXT("Healing! Current Health: %f"), CurrentHealth);
	}
}
