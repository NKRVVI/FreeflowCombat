// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTAttack.h"
#include "Characters/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTAttack::UBTTAttack()
{
	NodeName = "Attack";
}


EBTNodeResult::Type UBTTAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Self = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));

	if (Self)
	{
		Self->Attack();
		Self->AttackEndDelegate.AddLambda([&]() {FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); });
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
