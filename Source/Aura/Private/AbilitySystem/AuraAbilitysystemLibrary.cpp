// Copyright Shawwy


#include "AbilitySystem/AuraAbilitysystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Aura_PlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystemComponent.h"

UOverlayWidgetController* UAuraAbilitysystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHud = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHud->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeWidgetController* UAuraAbilitysystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldConextObject)
{

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldConextObject, 0))
	{
		if (AAuraHUD* AuraHud = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			
			return AuraHud->GetAttributeWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UAuraAbilitysystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC)
{

	AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	AActor* AvatarActor = ASC->GetAvatarActor();
	if (GameMode == nullptr) return;

	FCharacterClassDefaultInfo ClassDefaultInfo = GameMode->CharacterInfo->GetClassDefaultInfomation(CharacterClass);

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle ESH = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ESH.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle ESH_Secondary = ASC->MakeOutgoingSpec(GameMode->CharacterInfo->SecondaryAttributes, level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ESH_Secondary.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle ESH_Vital = ASC->MakeOutgoingSpec(GameMode->CharacterInfo->VitalAttributes, level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ESH_Vital.Data.Get());
}

void UAuraAbilitysystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{

	AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	UCharacterClassInfo* ClassDefaultInfo = GameMode->CharacterInfo;
	
	for (auto Ability : ClassDefaultInfo->Abilities)
	{

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
	
		AbilitySpec.Ability->AbilityTags;

		if (ASC->GiveAbility(AbilitySpec).IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Success"));
		}
	}

}
