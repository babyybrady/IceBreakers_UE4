#pragma once

#include "IceBreakers.h"
#include "IBAttributeSet.h"
#include "GameplayEffectExecutionCalculation.h"
#include "IBDamageExecution.generated.h"

struct IBExecutionMgr
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	IBExecutionMgr()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UIBAttributeSet, DefensePower, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UIBAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UIBAttributeSet, Damage, Source, true);
	}
};

UCLASS()
class ICEBREAKERS_API UIBDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:

	UIBDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
