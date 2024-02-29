// Copyright Shawwy


#include "AbilitySystem/ModMagClac/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/Combat_Interface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//Capture the Vigor attributes
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	//Choose the attribute source
	//The effect will use the target's Vigor attribute in its calculations. 
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	//Decide whether if it's snapshot
	VigorDef.bSnapshot = false;


	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	ICombat_Interface* CombatInterface = Cast<ICombat_Interface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
