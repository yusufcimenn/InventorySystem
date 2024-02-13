// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_XGameMode.h"
#include "Project_XCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_XGameMode::AProject_XGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
