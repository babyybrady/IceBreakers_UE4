// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IceBreakers.h"
#include "IBAttributeSet.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CooldownDuration.generated.h"

/**
 * 
 */
UCLASS()
class ICEBREAKERS_API UMMC_CooldownDuration : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FGameplayEffectAttributeCaptureDefinition CooldownSpeedDef;

public:
	UMMC_CooldownDuration(); 
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;
};
