// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "VaraStyle.h"

class FVaraCommands : public TCommands<FVaraCommands>
{
public:

	FVaraCommands()
		: TCommands<FVaraCommands>(TEXT("Vara"), NSLOCTEXT("Contexts", "Vara", "Vara Plugin"), NAME_None, FVaraStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};