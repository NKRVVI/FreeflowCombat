// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Characters/ActionStates.h"
#include "FreeflowCombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFreeflowCombatCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/* attack input action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/* counter input action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CounterAction;

public:
	AFreeflowCombatCharacter();

	virtual void Tick(float DeltaTime) override;
	void GetHit(FVector ImpactDirection);
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void NotMoving(const FInputActionValue& Value);

	virtual void Jump() override;

	void Counter(const FInputActionValue& Value);

	void PlayMontageSection(UAnimMontage* Montage, FName SectionName);
	void PlayRandomMontageSection(UAnimMontage* Montage);

	TArray<class AEnemy*> Enemies;

	UPROPERTY(EditDefaultsOnly)
	float EnemySelectionErrorRate;
			
	UPROPERTY(BlueprintReadOnly)
	class UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAttacking;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void HitLanded();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void CounterEnd();

	UFUNCTION(BlueprintCallable)
	void CounterLanded();

	UFUNCTION(BlueprintCallable)
	void BlockAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFreeflowActionState ActionState = EFreeflowActionState::EFAS_Unoccupied;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void WarpToTarget();
	FVector GetTranslationWarpTarget(AActor* Target);
	FVector GetTranslationWarpTarget(FVector Target);
	FVector GetRotationWarpTarget(AActor* Target);
	FVector GetRotationWarpTarget(FVector Target);

	UPROPERTY()
	class AEnemy* CombatTarget;

	UPROPERTY()
	AEnemy* LastCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPunchType CurrentPunchType = EPunchType::EPT_Standard;

	UPROPERTY(VisibleAnywhere)
	EPunchType LastPunchType = EPunchType::EPT_Standard;

	UPROPERTY(BlueprintReadOnly)
	AEnemy* CounterTarget;

	UPROPERTY(EditAnywhere)
	double WarpTargetDistance = 75.f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FreeflowAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* StandardAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* KnockoutAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* CounterMontage;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* HitSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* KnockoutSound;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE void SetCounterTarget(AEnemy* Target) { CounterTarget = Target; }
	FORCEINLINE AEnemy* GetCounterTarget() const { return CounterTarget; }
	FORCEINLINE EFreeflowActionState GetActionState() const { return ActionState; }
};

