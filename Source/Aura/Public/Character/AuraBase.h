// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraBase.generated.h"

UCLASS()
class AURA_API AAuraBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraBase();

protected:
	virtual void BeginPlay() override;

};
