// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBase.h"
#include "Interaction/Enemy_Interface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraBase , public IEnemy_Interface
{
	GENERATED_BODY()
	
public:

	AAuraEnemy();


	virtual void HightlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
};
