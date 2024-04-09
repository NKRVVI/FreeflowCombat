// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class FREEFLOWCOMBAT_API AEnemyController : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* PossessPawn);

protected:
	class AEnemy* PossessedEnemy;
	class AFreeflowCombatCharacter* Player;

	class UBehaviorTree* BTAsset;

public:

	virtual void Tick(float DeltaTime) override;
	
};
