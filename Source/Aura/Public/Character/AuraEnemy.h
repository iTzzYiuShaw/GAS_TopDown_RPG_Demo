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

	/** Enemy Interface */
	virtual void HightlightActor() override;
	virtual void UnHighlightActor() override;
	/** End  Enemy Interface */


protected:
	virtual void BeginPlay() override;

};
