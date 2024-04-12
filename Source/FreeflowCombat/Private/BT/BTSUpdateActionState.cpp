// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTSUpdateActionState.h"
#include "Characters/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters//ActionStates.h"

UBTSUpdateActionState::UBTSUpdateActionState()
{
	NodeName = "Update Action State";
}

void UBTSUpdateActionState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Source = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SourceKey.SelectedKeyName));

	if (Source)
	{
		EEnemyActionState ActionState = Source->GetActionState();
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActionState"), static_cast<uint8>(ActionState));
	}
}

