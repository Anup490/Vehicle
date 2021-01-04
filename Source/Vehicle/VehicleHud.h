// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "VehicleHud.generated.h"


UCLASS(config = Game)
class AVehicleHud : public AHUD
{
	GENERATED_BODY()

public:
	AVehicleHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
