// Copyright Epic Games, Inc. All Rights Reserved.

#include "VaraCommands.h"

#define LOCTEXT_NAMESPACE "FVaraModule"

void FVaraCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Vara", "Bring up Vara window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
