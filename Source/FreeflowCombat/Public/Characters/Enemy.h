// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ActionStates.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackEndSignature)

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
	void StopAttack();
	USceneComponent* GetComponentWithTag(FName Tag);

	void Attack();

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BTAsset;

	FAttackEndSignature AttackEndDelegate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayMontageSection(UAnimMontage* Montage, FName SectionName);
	void PlayRandomMontageSection(UAnimMontage* Montage);
	bool WithinDistance(AActor* Target, float Distance);
	bool WithinDistance(FVector TargetLocation, float Distance);

	void MoveToTarget(AActor* Target);
	void MoveToTarget(FVector TargetLocation);
	void PossessedBy(AController* NewController);

	void WarpToTarget();
	FVector GetTranslationWarpTarget(AActor* Target);
	FVector GetRotationWarpTarget(AActor* Target);
	void ClearAttackTimer();
	double GetDirectionalTheta(FVector impact_point);
	FName GetDirectionalSectionName(FVector ImpactPoint);
	void UpdateActionState();

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
	EEnemyActionState ActionState = EEnemyActionState::EEAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyCombatState CombatState = EEnemyCombatState::EECS_Circling;

	TArray<USceneComponent*> CounterPoints;

	class UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly)
	float AttackRadius = 150.f;
	FTimerHandle AttackTimer;

	UPROPERTY(EditDefaultsOnly)
	float AttackTime = 2.f;

	FVector BaseLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EPunchType LastReceivedPunch = EPunchType::EPT_NoPunch;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE EEnemyActionState GetActionState() const { return ActionState; }
	FORCEINLINE void SetActionState(EEnemyActionState State) {ActionState = State;}
	FORCEINLINE void SetCombatState(EEnemyCombatState State) { CombatState = State; }
	FORCEINLINE EEnemyCombatState GetCombatState() const { return CombatState; }
	FORCEINLINE EPunchType GetLastReceivedPunch() const { return LastReceivedPunch; }
};
