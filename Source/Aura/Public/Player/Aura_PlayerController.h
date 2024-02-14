// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Aura_PlayerController.generated.h"



class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemy_Interface;
/**
 * 
 */
UCLASS()
class AURA_API AAura_PlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	AAura_PlayerController();
	virtual void PlayerTick(float DeltaTime) override;
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	TObjectPtr<IEnemy_Interface> LastActor;
	TObjectPtr<IEnemy_Interface> ThisActor;
};
