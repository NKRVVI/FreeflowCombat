// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSDistance.generated.h"

/**
 * 
 */
UCLASS()
class FREEFLOWCOMBAT_API UBTSDistance : public UBTService
{
	GENERATED_BODY()

	UBTSDistance();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceKey;
	
};
