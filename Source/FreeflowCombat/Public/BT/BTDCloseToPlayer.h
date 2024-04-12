// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_IsAtLocation.h"
#include "BTDCloseToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FREEFLOWCOMBAT_API UBTDCloseToPlayer : public UBTDecorator_IsAtLocation
{
	GENERATED_BODY()
	
	UBTDCloseToPlayer() : Super()
	{
		bAllowAbortLowerPri = true;
		bAllowAbortNone = true;
		bAllowAbortChildNodes = true;
	}
};
