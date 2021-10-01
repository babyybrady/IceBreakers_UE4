#pragma once

#include "IceBreakers.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "EffectContainer.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UIBTargetType;

USTRUCT(BlueprintType)
struct FIBGameplayEffectContainer
{
	GENERATED_BODY()
public:
	FIBGameplayEffectContainer() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TSubclassOf<UIBTargetType> TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

USTRUCT(BlueprintType)
struct FIBGameplayEffectContainerSpec
{
	GENERATED_BODY()
public:
	FIBGameplayEffectContainerSpec() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	bool HasValidEffects() const;

	bool HasValidTargets() const;

	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};
