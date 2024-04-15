#pragma once

UENUM(BlueprintType)
enum class EFreeflowActionState : uint8
{
	EFAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EFAS_Attacking UMETA(DisplayName = "Attacking"),
	EFAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EFAS_Counter UMETA(DisplayName = "Counter")
};

UENUM(BlueprintType)
enum class EEnemyActionState : uint8
{
	EEAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EEAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EEAS_Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	EECS_Circling UMETA(DisplayName = "Circling"),
	EECS_Chasing UMETA(DisplayName = "Chasing"),
	EECS_Retreating UMETA(DisplayName = "Retreating"),
	EECS_Down UMETA(DisplayName = "Down")
};

UENUM(BlueprintType)
enum class EPunchType : uint8
{
	EPT_NoPunch UMETA(DisplayName = "No Punch"),
	EPT_Freeflow UMETA(DisplayName = "Freeflow Punch"),
	EPT_Standard1 UMETA(DisplayName = "Standard Punch 1"),
	EPT_Standard2 UMETA(DisplayName = "Standard Punch 2"),
	EPT_Knockout UMETA(DisplayName = "Knockout Punch")
};