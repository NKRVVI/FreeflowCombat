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
	EEAS_Dead UMETA(DisplayName = "Dead"),
	EEAS_Down UMETA(DisplayName = "Down"),
	EEAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EEAS_Chasing UMETA(DisplayName = "Chasing"),
	EEAS_Engaged UMETA(DisplayName = "Engaged"),
	EEAS_Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class EPunchType : uint8
{
	EPT_Standard UMETA(DisplayName = "Standard Punch"),
	EPT_Freeflow UMETA(DisplayName = "Freeflow Punch"),
	EPT_Knockout UMETA(DisplayName = "Knockout Punch")
};