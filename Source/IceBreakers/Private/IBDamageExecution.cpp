#include "IBDamageExecution.h"

static const IBExecutionMgr& ExecutionMgr()
{
	static IBExecutionMgr ExecutionManager;
	return ExecutionManager;
}

UIBDamageExecution::UIBDamageExecution()
{
	RelevantAttributesToCapture.Add(ExecutionMgr().DefensePowerDef);
	RelevantAttributesToCapture.Add(ExecutionMgr().AttackPowerDef);
	RelevantAttributesToCapture.Add(ExecutionMgr().DamageDef);
}

void UIBDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float DefensePower{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ExecutionMgr().DefensePowerDef, EvaluationParameters, DefensePower);

	float AttackPower{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ExecutionMgr().AttackPowerDef, EvaluationParameters, AttackPower);

	float Damage{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ExecutionMgr().DamageDef, EvaluationParameters, Damage);

	float DamageDone{};
	if (DefensePower == 0.f)
	{
		DamageDone = 0.f;
	}

	else DamageDone = Damage * AttackPower / DefensePower;
	
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ExecutionMgr().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}
