// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager/EnemyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Enemy.h"
#include "Characters/ActionStates.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Actors);
	for (auto Actor : Actors)
	{
		if (AEnemy* Enemy = Cast<AEnemy>(Actor))
		{
			Enemies.Add(Enemy);
			Enemy->AttackEndDelegate.AddUObject(this, &ThisClass::ChooseChasingEnemy);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Number of enemies %d"), Enemies.Num());
	ChooseChasingEnemy();
}

void AEnemyManager::ChooseChasingEnemy()
{

	/*
	Fisher-Yates shuffle algorithm in order to shuffle the tiles and choose the ones that will become obstacles
	-- To shuffle an array a of n elements (indices 0..n-1):
	for i from n−1 down to 1 do
		j ← random integer such that 0 ≤ j ≤ i
		exchange a[j] and a[i]
	*/

	for (int i = Enemies.Num() - 1; i >= 1; i--)
	{
		int j = FMath::RandRange(0, i);
		AEnemy* SwapEnemy = Enemies[i];
		Enemies[i] = Enemies[j];
		Enemies[j] = SwapEnemy;
	}

	for (auto Enemy : Enemies)
	{
		if (Enemy->GetCombatState() == EEnemyCombatState::EECS_Circling)
		{
			Enemy->SetCombatState(EEnemyCombatState::EECS_Chasing);
			UE_LOG(LogTemp, Warning, TEXT("Enemy chosen"));
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No enemy chosen"));
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

