// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_UsePrimarySkill.h"

#include "AIController.h"
#include "BaseEnemy.h"

UBTTask_UsePrimarySkill::UBTTask_UsePrimarySkill()
{
	NodeName = TEXT("Use Primary Skill");
}

EBTNodeResult::Type UBTTask_UsePrimarySkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Failed;
	// Get AI Enemy
	AAIController* AIController = OwnerComp.GetAIOwner();
	ABaseEnemy* AIEnemy = static_cast<ABaseEnemy*>(AIController->GetPawn());

	// Use primary skill
	if (AIEnemy)
	{
		bool bResult = AIEnemy->UsePrimarySkill();
		if (bResult)
		{
			TaskResult = EBTNodeResult::Succeeded;
		}
	}

	// Signal to the BehaviorTreeComponent that the task is finished
	FinishLatentTask(OwnerComp, TaskResult);
	return TaskResult;
}

void UBTTask_UsePrimarySkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{


}

FString UBTTask_UsePrimarySkill::GetStaticDescription() const
{
	return FString();
}
