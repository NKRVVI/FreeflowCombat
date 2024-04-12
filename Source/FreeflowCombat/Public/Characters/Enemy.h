// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ActionStates.h"
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

	void GetHit(FVector ImpactDirection, EPunchType Punch);
	void GetCountered();
	USceneComponent* GetComponentWithTag(FName Tag);

	void Attack();

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BTAsset;

	FAttackEndDelegate AttackEndDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayMontageSection(UAnimMontage* Montage, FName SectionName);
	void PlayRandomMontageSection(UAnimMontage* Montage);
	bool WithinDistance(AActor* Target, float Distance);
	void UpdateActionState();
	void MoveToTarget(AActor* Target);
	void PossessedBy(AController* NewController);

	void WarpToTarget();
	FVector GetTranslationWarpTarget(AActor* Target);
	FVector GetRotationWarpTarget(AActor* Target);
	void ClearAttackTimer();
	double GetDirectionalTheta(FVector impact_point);
	FName GetDirectionalSectionName(FVector ImpactPoint);

	UPROPERTY(EditAnywhere)
	double WarpTargetDistance = 75.f;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void GetupEnd();

	UFUNCTION(BlueprintCallable)
	void HitLanded();

	UFUNCTION(BlueprintCallable)
	void CounteredEnd();

	UFUNCTION(BlueprintCallable)
	void SetPlayerCounterTarget();

	UFUNCTION(BlueprintCallable)
	void ResetPlayerCounterTarget();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactFallMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactSmallMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactLargeMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* GetUpMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* CounteredMontage;

	FTimerHandle GetUpTimer;
	void GetUp(FName SectionName);
	
	UPROPERTY(EditDefaultsOnly)
	float GetUpTime = 1.f;

	class AFreeflowCombatCharacter* Player;
	class AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyActionState ActionState = EEnemyActionState::EEAS_Chasing;

	TArray<USceneComponent*> CounterPoints;

	class UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly)
	float AttackRadius = 150.f;
	FTimerHandle AttackTimer;

	UPROPERTY(EditDefaultsOnly)
	float AttackTime = 2.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE EEnemyActionState GetActionState() const { return ActionState; }

};
