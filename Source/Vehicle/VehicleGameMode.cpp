// Copyright Epic Games, Inc. All Rights Reserved.

#include "VehicleGameMode.h"
#include "VehiclePawn.h"
#include "VehicleHud.h"

AVehicleGameMode::AVehicleGameMode()
{
	DefaultPawnClass = AVehiclePawn::StaticClass();
	HUDClass = AVehicleHud::StaticClass();
}
