// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_CooldownDuration_AttackSpeed.h"

UMMC_CooldownDuration_AttackSpeed::UMMC_CooldownDuration_AttackSpeed()
{
	AttackSpeedDef.AttributeToCapture = UIBAttributeSet::GetAttackSpeedAttribute();
	AttackSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackSpeedDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(AttackSpeedDef);
}

float UMMC_CooldownDuration_AttackSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AttackSpeed = 0.f;
	GetCapturedAttributeMagnitude(AttackSpeedDef, Spec, EvaluationParameters, AttackSpeed);
	AttackSpeed = FMath::Max<float>(AttackSpeed, 0.1f);

	float ActualDuration = 1.f / AttackSpeed;

	return ActualDuration;
}
