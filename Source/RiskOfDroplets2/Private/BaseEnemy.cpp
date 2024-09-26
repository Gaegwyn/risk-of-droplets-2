// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AI/BaseEnemyAIController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;				// Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);	// ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 1.0f;	// Full control at max speed of MaxWalkSpeed					
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	// Setup damage system component
	DamageSystemComponent = CreateDefaultSubobject<UDamageSystemComponent>(TEXT("DamageSystem"));
	DamageSystemComponent->MaxHealth = 100.0f;
	DamageSystemComponent->CurrentHealth = DamageSystemComponent->MaxHealth;

	// Default our AI controller to our custom controller
	AIControllerClass = ABaseEnemyAIController::StaticClass();
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

const TObjectPtr<UBehaviorTree> ABaseEnemy::GetBehaviorTree() const
{
	return BehaviorTreeAsset;
}

bool ABaseEnemy::UsePrimarySkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Primary Skill!"))
	return true;
}

bool ABaseEnemy::UseSecondarySkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Secondary Skill!"))
	return true;
}

bool ABaseEnemy::UseUtilitySkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Utility Skill!"))
	return true;
}

bool ABaseEnemy::UseSpecialSkill()
{
	//UE_LOG(LogTemp, Log, TEXT("Using BaseCharacter's Special Skill!"))
	return true;
}

void ABaseEnemy::ResetPrimarySkillCooldown()
{
	bCanUsePrimarySkill = true;
}

void ABaseEnemy::ResetSecondarySkillCooldown()
{
	bCanUseSecondarySkill = true;
}

void ABaseEnemy::ResetUtilitySkillCooldown()
{
	bCanUseUtilitySkill = true;
}

void ABaseEnemy::ResetSpecialSkillCooldown()
{
	bCanUseSpecialSkill = true;
}

