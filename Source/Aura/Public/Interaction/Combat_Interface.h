// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combat_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombat_Interface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;
/**
 * 
 */
class AURA_API ICombat_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float MaxHealthMagnitudeCalculation();

	virtual float MaxManaMagnitudeCalculation();

	virtual int32 GetPlayerLevel();

	virtual FVector GetCombatSocketLocation(); 

	virtual void Die() = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);


	//If this function is BlueprintImplementableEvent, then we cannot override it in C++
	//BlueprintNativeEvent automatically generate a virtual native version that exists in C++ 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();



};
