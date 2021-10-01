#pragma once

#include "IceBreakers.h"
#include "IBAttributeSet.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CooldownDuration_AttackSpeed.generated.h"

/**
 *
 */
UCLASS()
class ICEBREAKERS_API UMMC_CooldownDuration_AttackSpeed : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FGameplayEffectAttributeCaptureDefinition AttackSpeedDef;

public:
	UMMC_CooldownDuration_AttackSpeed();
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;
};
