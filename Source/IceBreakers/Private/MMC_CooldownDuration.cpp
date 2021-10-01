// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_CooldownDuration.h"


UMMC_CooldownDuration::UMMC_CooldownDuration()
{
	CooldownSpeedDef.AttributeToCapture = UIBAttributeSet::GetCooldownSpeedAttribute();
	CooldownSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CooldownSpeedDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(CooldownSpeedDef);
}

float UMMC_CooldownDuration::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float CooldownSpeed = 0.f;
	GetCapturedAttributeMagnitude(CooldownSpeedDef, Spec, EvaluationParameters, CooldownSpeed);
	CooldownSpeed = FMath::Max<float>(CooldownSpeed, 0.1f); // Avoid divide by zero

	float ActualDuration = 1.f / CooldownSpeed;

	return ActualDuration;
}
