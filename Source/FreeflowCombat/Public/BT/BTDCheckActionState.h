// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Characters/ActionStates.h"
#include "BTDCheckActionState.generated.h"

/**
 * 
 */
UCLASS()
class FREEFLOWCOMBAT_API UBTDCheckActionState : public UBTDecorator
{
	GENERATED_BODY()

	UBTDCheckActionState();
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ActionStateKey;

	UPROPERTY(EditAnywhere)
	EEnemyActionState ActionState = EEnemyActionState::EEAS_Chasing;

};
