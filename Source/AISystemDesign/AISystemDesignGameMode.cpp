// Copyright Epic Games, Inc. All Rights Reserved.

#include "AISystemDesignGameMode.h"
#include "AISystemDesignCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAISystemDesignGameMode::AAISystemDesignGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
