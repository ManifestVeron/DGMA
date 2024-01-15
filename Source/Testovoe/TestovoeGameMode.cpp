// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestovoeGameMode.h"
#include "TestovoeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestovoeGameMode::ATestovoeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
