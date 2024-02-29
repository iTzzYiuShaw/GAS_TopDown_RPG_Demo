// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBase.h"
#include "Interaction/Enemy_Interface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
class UWidgetComponent;

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


	/** Combat Interface */

	virtual int32 GetPlayerLevel() override;

	/** Combat Interface */



	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	/*
	* Hit react: Listen for gameplay tag changing on ability system component
	* 1: Binding a delegate
	*/
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void Die() override;




	//TODO: Network Sychronization?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlueprintReadOnly")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlueprintReadOnly")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

};
