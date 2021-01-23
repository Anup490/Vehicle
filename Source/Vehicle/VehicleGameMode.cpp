// Copyright Epic Games, Inc. All Rights Reserved.

#include "VehicleGameMode.h"
#include "VehiclePawn.h"
#include "Muffin.h"
#include "VehicleHud.h"

AVehicleGameMode::AVehicleGameMode()
{
	DefaultPawnClass = AMuffin::StaticClass();
	HUDClass = AVehicleHud::StaticClass();
}
