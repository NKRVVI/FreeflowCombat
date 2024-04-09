// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackEndDelegate)

UCLASS()
class FREEFLOWCOMBAT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEnemyHUD(float Angle);

	void GetHit(FVector ImpactDirection);

	void Attack();

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BTAsset;

	FAttackEndDelegate AttackEndDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayMontageSection(UAnimMontage* Montage, FName SectionName);
	void PlayRandomMontageSection(UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* GetUpMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	FTimerHandle GetUpTimer;
	void GetUp();
	
	UPROPERTY(EditDefaultsOnly)
	float GetUpTime = 1.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
