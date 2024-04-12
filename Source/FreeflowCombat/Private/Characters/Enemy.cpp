// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/FreeflowCombatCharacter.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AEnemy::GetHit(FVector ImpactDirection, EPunchType PunchType)
{
	ClearAttackTimer();
	FName SectionName = GetDirectionalSectionName(ImpactDirection);

	if (PunchType == EPunchType::EPT_Standard)
	{
		ActionState = EEnemyActionState::EEAS_HitReaction;
		PlayMontageSection(HitReactSmallMontage, SectionName);
		UE_LOG(LogTemp, Warning, TEXT("Standard"));
	}
	else if (PunchType == EPunchType::EPT_Freeflow)
	{
		ActionState = EEnemyActionState::EEAS_HitReaction;
		PlayMontageSection(HitReactLargeMontage, SectionName);
		UE_LOG(LogTemp, Warning, TEXT("Freeflow"));
	}
	else
	{
		ActionState = EEnemyActionState::EEAS_Down;
		PlayMontageSection(HitReactFallMontage, SectionName);
		FTimerDelegate GetupTimerDelegate;
		GetupTimerDelegate.BindUObject(this, &ThisClass::GetUp, SectionName);
		GetWorldTimerManager().SetTimer(GetUpTimer, GetupTimerDelegate, GetUpTime, false);
		UE_LOG(LogTemp, Warning, TEXT("Knockout"));
	}
}

void AEnemy::GetCountered()
{
	PlayRandomMontageSection(CounteredMontage);
	ActionState = EEnemyActionState::EEAS_Down;
}

USceneComponent* AEnemy::GetComponentWithTag(FName Tag)
{
	for (auto& Component : CounterPoints)
	{
		if (Component->ComponentHasTag(Tag))
		{
			return Component;
		}
	}

	return nullptr;
}

void AEnemy::Attack()
{
	ActionState = EEnemyActionState::EEAS_Attacking;
	PlayRandomMontageSection(AttackMontage);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetComponents(USceneComponent::StaticClass(), CounterPoints);

	Player = Cast<AFreeflowCombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AEnemy::PlayMontageSection(UAnimMontage* Montage, FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AEnemy::PlayRandomMontageSection(UAnimMontage* Montage)
{
	if (!Montage) return;

	int32 NumSection = Montage->CompositeSections.Num();
	int32 RandomSelection = FMath::RandRange(0, NumSection - 1);
	FName RandomSectionName = Montage->GetSectionName(RandomSelection);

	PlayMontageSection(Montage, RandomSectionName);
}

void AEnemy::AttackEnd()
{
	AttackEndDelegate.Broadcast();
	ActionState = EEnemyActionState::EEAS_Chasing;
}

void AEnemy::GetupEnd()
{
	AttackEndDelegate.Broadcast();
	ActionState = EEnemyActionState::EEAS_Chasing;
}

void AEnemy::HitLanded()
{
	AFreeflowCombatCharacter* Character = Cast<AFreeflowCombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Character && Character->GetActionState() != EFreeflowActionState::EFAS_Counter)
	{
		Character->GetHit(FVector());
	}
}

void AEnemy::CounteredEnd()
{
	GetUp(FName());
}

void AEnemy::SetPlayerCounterTarget()
{
	AFreeflowCombatCharacter* Character = Cast<AFreeflowCombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character && !Character->GetCounterTarget())
	{
		Character->SetCounterTarget(this);
	}
}

void AEnemy::ResetPlayerCounterTarget()
{
	AFreeflowCombatCharacter* Character = Cast<AFreeflowCombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character && Character->GetCounterTarget() == this && Character->GetActionState() != EFreeflowActionState::EFAS_Counter)
	{
		Character->SetCounterTarget(nullptr);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
	}
}

void AEnemy::HitReactEnd()
{
	ActionState = EEnemyActionState::EEAS_Chasing;
}

void AEnemy::GetUp(FName SectionName)
{
	/*GetMesh()->SetSimulatePhysics(false);
	PlayMontageSection(GetUpMontage, SectionName);*/
}

bool AEnemy::WithinDistance(AActor* Target, float Distance)
{
	if (!Target) return false;
	return UKismetMathLibrary::VSizeXY(GetActorLocation() - Target->GetActorLocation()) <= Distance;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (!EnemyController || !Target) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EnemyController = Cast<AAIController>(NewController);
}

void AEnemy::WarpToTarget()
{

	if (Player && MotionWarpingComponent && ActionState == EEnemyActionState::EEAS_Attacking)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("TranslationTarget"), GetTranslationWarpTarget(Player));
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("RotationTarget"), GetRotationWarpTarget(Player));

	}
	else
	{
		MotionWarpingComponent->RemoveWarpTarget(FName("TranslationTarget"));
		MotionWarpingComponent->RemoveWarpTarget(FName("RotationTarget"));
	}
}

FVector AEnemy::GetTranslationWarpTarget(AActor* Target)
{
	if (!Target) return FVector();

	const FVector CombatTargetLocation = Target->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;
}

FVector AEnemy::GetRotationWarpTarget(AActor* Target)
{
	if (Target)
	{
		return Target->GetActorLocation();
	}
	return FVector();
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

double AEnemy::GetDirectionalTheta(FVector impact_point)
{
	FVector impact_point_lowered = FVector(impact_point.X, impact_point.Y, GetActorLocation().Z);
	FVector impact_vector = (impact_point_lowered - GetActorLocation()).GetSafeNormal();
	FVector actor_vector = GetActorForwardVector();

	double theta = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(actor_vector, impact_vector)));

	FVector cross_product = FVector::CrossProduct(actor_vector, impact_vector);
	if (cross_product.Z < 0)
	{
		theta *= -1;
	}

	return theta;
}

FName AEnemy::GetDirectionalSectionName(FVector ImpactPoint)
{
	double Theta = GetDirectionalTheta(ImpactPoint);

	FName Section("FromBack");

	if (Theta >= -45 && Theta < 45)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135 && Theta < -45)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45 && Theta < 135)
	{
		Section = FName("FromRight");
	}

	return Section;
}


void AEnemy::UpdateActionState()
{
	if (ActionState < EEnemyActionState::EEAS_Chasing) return;

	if (!WithinDistance(Player, AttackRadius) && ActionState != EEnemyActionState::EEAS_Attacking)
	{
		ClearAttackTimer();
		MoveToTarget(Player);
		ActionState = EEnemyActionState::EEAS_Chasing;
		UE_LOG(LogTemp, Warning, TEXT("Chasing"));
	}
	else if(ActionState < EEnemyActionState::EEAS_Engaged)
	{
		//start attack timer
		ClearAttackTimer();
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		ActionState = EEnemyActionState::EEAS_Engaged;
		UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateActionState();

	WarpToTarget();

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

