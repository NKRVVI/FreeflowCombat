// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSUpdateActionState.generated.h"

/**
 * 
 */
UCLASS()
class FREEFLOWCOMBAT_API UBTSUpdateActionState : public UBTService
{
	GENERATED_BODY()

	UBTSUpdateActionState();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SourceKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ActionStateKey;
	
};