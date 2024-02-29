// Copyright Shawwy


#include "AbilitySystem/ModMagClac/MMC_MaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/Combat_Interface.h"

UMMC_MaxMana::UMMC_MaxMana()
{

	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();

	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	IntelligenceDef.bSnapshot = false;
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();

	const FGameplayTagContainer* TargetTag = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParamters;
	EvaluationParamters.SourceTags = SourceTag;
	EvaluationParamters.TargetTags = TargetTag;

	float intelligence = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParamters, intelligence);
	intelligence = FMath::Max<float>(intelligence, 0.f);

	ICombat_Interface* CombatInterface = Cast<ICombat_Interface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 3.5 * intelligence + 11.f * PlayerLevel;
}
