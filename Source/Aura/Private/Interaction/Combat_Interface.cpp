// Copyright Shawwy


#include "Interaction/Combat_Interface.h"

// Add default functionality here for any ICombat_Interface functions that are not pure virtual.

float ICombat_Interface::MaxHealthMagnitudeCalculation()
{
	return 0.0f;
}

float ICombat_Interface::MaxManaMagnitudeCalculation()
{
	return 0.0f;
}

int32 ICombat_Interface::GetPlayerLevel()
{
	return 0;
}

FVector ICombat_Interface::GetCombatSocketLocation()
{
	return FVector();
}
