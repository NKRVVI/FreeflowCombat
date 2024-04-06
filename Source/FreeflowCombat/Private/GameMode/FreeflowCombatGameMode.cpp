// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/FreeflowCombatGameMode.h"
#include "Characters/FreeflowCombatCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFreeflowCombatGameMode::AFreeflowCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
