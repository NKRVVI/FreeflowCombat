// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyController.h"
#include "Characters/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/FreeflowCombatCharacter.h"

void AEnemyController::OnPossess(APawn* PossessPawn)
{
	Super::OnPossess(PossessPawn);

	PossessedEnemy = Cast<AEnemy>(PossessPawn);
	BTAsset = PossessedEnemy->BTAsset;
	Player = Cast<AFreeflowCombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PossessedEnemy && BTAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Running behaviour tree"));
		RunBehaviorTree(BTAsset);
		GetBlackboardComponent()->SetValueAsObject("Player", Player);
		GetBlackboardComponent()->SetValueAsObject("Self", PossessedEnemy);
	}
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player)
	{
		Player = Cast<AFreeflowCombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (Player)
		{
			GetBlackboardComponent()->SetValueAsObject("Player", Player);
		}
	}

}
