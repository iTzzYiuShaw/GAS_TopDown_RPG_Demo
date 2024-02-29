// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBase.h"
#include "Player/AuraPlayerState.h"
#include "Player/Aura_PlayerController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "UI/HUD/AuraHud.h"
#include "AbilitySystemComponent.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


	/** Combat Interface */
	
	virtual int32 GetPlayerLevel() override;


	/** Combat Interface */

private:
	virtual void InitAbilityActorInfo() override;
};
