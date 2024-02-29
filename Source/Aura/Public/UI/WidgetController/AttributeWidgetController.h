// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Abilitysystem/Data/AttributeInfo.h"
#include "Delegates/DelegateCombinations.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AttributeWidgetController.generated.h"

/**
 * 
 */
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);




UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;


protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	void BroadcastInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute);
};
