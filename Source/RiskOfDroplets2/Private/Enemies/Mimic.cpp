// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Mimic.h"


// Sets default values
AMimic::AMimic()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup our damage system
	GetDamageSystem()->MaxHealth = 350.0f;
	GetDamageSystem()->CurrentHealth = GetDamageSystem()->MaxHealth;

	// Can use abilities
	bCanUsePrimarySkill = true;
	bCanUseSecondarySkill = true;
	bCanUseUtilitySkill = true;
	bCanUseSpecialSkill = true;

	// Cooldowns
	PrimarySkillCooldown = 0.5f;
	SecondarySkillCooldown = 7.0f;
	UtilitySkillCooldown = 7.0f;
	SpecialSkillCooldown = 12.0f;
}

// Called when the game starts or when spawned
void AMimic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMimic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMimic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMimic::UsePrimarySkill()
{
	if (!bCanUsePrimarySkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Mimic's primary skill on cooldown!"));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Using Mimic's Primary Skill!"));

	// Do Primary skill...
	// ...

	// Go on cooldown
	bCanUsePrimarySkill = false;
	GetWorld()->GetTimerManager().SetTimer(PrimarySkillTimerHandle, this, &ABaseEnemy::ResetPrimarySkillCooldown, PrimarySkillCooldown, true);

	return true;
}

bool AMimic::UseSecondarySkill()
{
	if (!bCanUseSecondarySkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Mimic's secondary skill on cooldown!"));
		return false;
	}

	// Do Secondary skill stuff


	return true;
}

bool AMimic::UseUtilitySkill()
{
	if (!bCanUseUtilitySkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Mimic's utility skill on cooldown!"));
		return false;
	}

	return true;
}

bool AMimic::UseSpecialSkill()
{
	if (!bCanUseSpecialSkill)
	{
		UE_LOG(LogTemp, Log, TEXT("Mimic's special skill on cooldown!"));
		return false;
	}

	return true;
}

