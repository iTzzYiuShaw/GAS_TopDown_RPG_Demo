// Copyright Shawwy


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
UAuraAssetManager& UAuraAssetManager::Get()
{

	UAuraAssetManager* AssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	//This is required to use Target Data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
