// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/FreeflowCombatCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Enemy.h"
#include "DrawDebugHelpers.h"
#include "MotionWarpingComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFreeflowCombatCharacter

AFreeflowCombatCharacter::AFreeflowCombatCharacter()
{
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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AFreeflowCombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WarpToTarget();
}

void AFreeflowCombatCharacter::GetHit(FVector ImpactDirection)
{
	ActionState = EFreeflowActionState::EFAS_HitReaction;
	PlayRandomMontageSection(HitReactMontage);
}

void AFreeflowCombatCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		Enemies.Add(Cast<AEnemy>(Actor));
	}
}

void AFreeflowCombatCharacter::WarpToTarget()
{
	/*if (CounterTarget && MotionWarpingComponent && ActionState == EFreeflowActionState::EFAS_Counter)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("TranslationTarget"), GetTranslationWarpTarget(CounterTarget));
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("RotationTarget"), GetRotationWarpTarget(CounterTarget));
		UE_LOG(LogTemp, Warning, TEXT("Warping to the component"));
		/*MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("TranslationTarget"), GetTranslationWarpTarget(CounterTarget));
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("RotationTarget"), GetRotationWarpTarget(CounterTarget));

	}*/
	if (CombatTarget && MotionWarpingComponent && (ActionState == EFreeflowActionState::EFAS_Attacking || ActionState == EFreeflowActionState::EFAS_Counter))
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("TranslationTarget"), GetTranslationWarpTarget(CombatTarget));
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("RotationTarget"), GetRotationWarpTarget(CombatTarget));

	}
	else
	{
		MotionWarpingComponent->RemoveWarpTarget(FName("TranslationTarget"));
		MotionWarpingComponent->RemoveWarpTarget(FName("RotationTarget"));

	}
}

FVector AFreeflowCombatCharacter::GetTranslationWarpTarget(AActor* Target)
{
	if (!Target) return FVector();

	const FVector CombatTargetLocation = Target->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;
}

FVector AFreeflowCombatCharacter::GetTranslationWarpTarget(FVector Target)
{
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - Target).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return Target + TargetToMe;
}



FVector AFreeflowCombatCharacter::GetRotationWarpTarget(AActor* Target)
{
	if (Target)
	{
		return Target->GetActorLocation();
	}
	return FVector();
}

FVector AFreeflowCombatCharacter::GetRotationWarpTarget(FVector Target)
{
	return Target;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFreeflowCombatCharacter::AttackEnd()
{
	bIsAttacking = false;
	CombatTarget = nullptr;
	ActionState = EFreeflowActionState::EFAS_Unoccupied;
}

void AFreeflowCombatCharacter::HitLanded()
{
	FVector Force = (CombatTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal() * 100;
	if (CurrentPunchType < EPunchType::EPT_Knockout && HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	else if (CurrentPunchType == EPunchType::EPT_Knockout && KnockoutSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, KnockoutSound, GetActorLocation());
	}
	CombatTarget->GetHit(GetActorLocation(), CurrentPunchType);
}

void AFreeflowCombatCharacter::HitReactEnd()
{
	ActionState = EFreeflowActionState::EFAS_Unoccupied;
}

void AFreeflowCombatCharacter::DodgeEnd()
{
	PlayRandomMontageSection(CounterMontage);
}

void AFreeflowCombatCharacter::CounterEnd()
{
	ActionState = EFreeflowActionState::EFAS_Unoccupied;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CounterTarget = nullptr;
}

void AFreeflowCombatCharacter::CounterLanded()
{
	if (CounterTarget)
	{
		CounterTarget->GetHit(GetActorLocation(), EPunchType::EPT_Knockout);
		UGameplayStatics::PlaySoundAtLocation(this, KnockoutSound, GetActorLocation());
		CounterTarget = nullptr;
	}
}

void AFreeflowCombatCharacter::BlockAttack()
{
	//CombatTarget->StopAttack();
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
}

void AFreeflowCombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFreeflowCombatCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::NotMoving);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFreeflowCombatCharacter::Look);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
		EnhancedInputComponent->BindAction(CounterAction, ETriggerEvent::Triggered, this, &ThisClass::Counter);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFreeflowCombatCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EFreeflowActionState::EFAS_Unoccupied) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *MovementVector.ToString());

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		//////////////////////////////////////////////////////////////////////////////////////////////
		//  FREEFLOW ENEMY SELECTION

		if (MovementVector == FVector2D::ZeroVector)
		{
			return;
		}

		FVector InputDirection = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal2D();
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + InputDirection * 1000, FColor::Orange, false);

		TArray<AActor*> ClosestEnemies;
		TArray<float> EnemyAngles;

		float ClosestAngle = 180.f;
		for (AEnemy* Enemy : Enemies)
		{
			if (Enemy->GetActionState() == EEnemyActionState::EEAS_Down) continue;

			FVector DirectionToEnemy = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			float EnemyAngle = FMath::Abs(FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(InputDirection, DirectionToEnemy))));
			Enemy->UpdateEnemyHUD(EnemyAngle);
			if (EnemyAngle < ClosestAngle)
			{
				ClosestEnemies.Empty();
				ClosestEnemies.Add(Enemy);
				ClosestAngle = EnemyAngle;
			}
			else if (EnemyAngle == ClosestAngle)
			{
				ClosestEnemies.Add(Enemy);
			}

		}

		CombatTarget = nullptr;
		for (auto Enemy : ClosestEnemies)
		{
			DrawDebugSphere(GetWorld(), Enemy->GetActorLocation(), 20, 12, FColor::Red, false);
			CombatTarget = Cast<AEnemy>(Enemy);
		}
	}
}

void AFreeflowCombatCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFreeflowCombatCharacter::Attack(const FInputActionValue& Value)
{
	if (!CombatTarget || ActionState != EFreeflowActionState::EFAS_Unoccupied || CombatTarget->GetActionState() == EEnemyActionState::EEAS_Down) return;

	UE_LOG(LogTemp, Warning, TEXT("Enemy state is %i"), CombatTarget->GetActionState());
	UAnimMontage* ChosenMontage = nullptr;

	if (!LastCombatTarget || LastCombatTarget != CombatTarget)
	{
		CurrentPunchType = EPunchType::EPT_Freeflow;
		ChosenMontage = FreeflowAttackMontage;
	}
	else if (LastCombatTarget == CombatTarget && CurrentPunchType == EPunchType::EPT_Freeflow)
	{
		CurrentPunchType = EPunchType::EPT_Standard;
		ChosenMontage = StandardAttackMontage;
	}
	else if (LastCombatTarget == CombatTarget && CurrentPunchType == EPunchType::EPT_Standard)
	{
		CurrentPunchType = EPunchType::EPT_Knockout;
		ChosenMontage = KnockoutAttackMontage;
	}

	LastCombatTarget = CombatTarget;

	bIsAttacking = true;
	ActionState = EFreeflowActionState::EFAS_Attacking;
	PlayRandomMontageSection(ChosenMontage);
}

void AFreeflowCombatCharacter::NotMoving(const FInputActionValue& Value)
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	if (ActionState == EFreeflowActionState::EFAS_Attacking) return;
	
	CombatTarget = nullptr;
}

void AFreeflowCombatCharacter::Jump()
{
	if (bIsAttacking) return;
	Super::Jump();
}

void AFreeflowCombatCharacter::Counter(const FInputActionValue& Value)
{
	if (!CounterTarget || ActionState == EFreeflowActionState::EFAS_Counter) return;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EFreeflowActionState::EFAS_Counter;
	CombatTarget = CounterTarget;
	//CounterTarget->GetCountered();


	PlayRandomMontageSection(DodgeMontage);
}

void AFreeflowCombatCharacter::PlayMontageSection(UAnimMontage* Montage, FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AFreeflowCombatCharacter::PlayRandomMontageSection(UAnimMontage* Montage)
{
	if (!Montage) return;

	int32 NumSection = Montage->CompositeSections.Num();
	int32 RandomSelection = FMath::RandRange(0, NumSection - 1);
	FName RandomSectionName = Montage->GetSectionName(RandomSelection);

	PlayMontageSection(Montage, RandomSectionName);
}
