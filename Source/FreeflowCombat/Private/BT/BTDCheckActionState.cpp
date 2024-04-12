// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTDCheckActionState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDCheckActionState::UBTDCheckActionState()
{
	NodeName = "Check Action State";
}

bool UBTDCheckActionState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	EEnemyActionState CurrentActionState = static_cast<EEnemyActionState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(ActionStateKey.SelectedKeyName));

	return CurrentActionState == ActionState;
}
