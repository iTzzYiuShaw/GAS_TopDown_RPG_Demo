// Copyright Shawwy


#include "UI/WidgetController/AttributeWidgetController.h"

#include "AuraGameplayTags.h"

void UAttributeWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastInfo(Pair.Key, Pair.Value());
	}

}

void UAttributeWidgetController::BindCallbacksToDependencies()
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AuraAttributeSet](const FOnAttributeChangeData& Data)
			{
				BroadcastInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeWidgetController::BroadcastInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AuraAttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

