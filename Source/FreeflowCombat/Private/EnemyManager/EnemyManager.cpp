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
	for (auto Enemy : Enemies)
	{
		if (Enemy->GetActionState() == EEnemyActionState::EEAS_Down) continue;
		Enemy->SetActionState(EEnemyActionState::EEAS_Chasing);
		break;
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

