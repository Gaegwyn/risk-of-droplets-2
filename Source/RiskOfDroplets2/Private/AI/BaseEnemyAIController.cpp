// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemy.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABaseEnemyAIController::ABaseEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<ABaseEnemy> Enemy = Cast<ABaseEnemy>(GetPawn());
	if (IsValid(Enemy.Get() ))
	{
		BehaviorTree = Enemy.Get()->GetBehaviorTree();
	}

	if (IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
	}
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Wait a moment to make sure everything we need is already initialized
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseEnemyAIController::InitializeBlackboard, 0.1f, false);
}

void ABaseEnemyAIController::InitializeBlackboard()
{
	if (IsValid(Blackboard.Get()) && IsValid(BehaviorTree.Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("Initializing AI Blackboard"));
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());

		// TODO: Consider moving blackboard to C++?? Making sure the names are exact seems annoying atm
		Blackboard->SetValueAsObject(TEXT("AttackTargetActor"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			UE_LOG(LogTemp, Log, TEXT("Player character is found!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Unable to find player character!"));
		}

	}
}
