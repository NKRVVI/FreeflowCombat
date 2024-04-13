// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class FREEFLOWCOMBAT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<class AEnemy*> Enemies;

	UFUNCTION()
	void ChooseChasingEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
