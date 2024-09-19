// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup Damage System component
	DamageSystemComponent = CreateDefaultSubobject<UDamageSystemComponent>(TEXT("DamageSystem"));
	DamageSystemComponent->MaxHealth = 100.0f;
	DamageSystemComponent->CurrentHealth = DamageSystemComponent->MaxHealth;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABaseEnemy::GetMaxHealth()
{
	return DamageSystemComponent->GetMaxHealth();
}

float ABaseEnemy::GetCurrentHealth()
{
	return DamageSystemComponent->GetCurrentHealth();
}

void ABaseEnemy::TakeDamage(const FDamageInfo& DamageInfo)
{
	DamageSystemComponent->TakeDamage(DamageInfo);
	// TODO: Check for player death if CurrentHealth <= 0
}

void ABaseEnemy::Heal(const float Value)
{
	DamageSystemComponent->Heal(Value);
}

