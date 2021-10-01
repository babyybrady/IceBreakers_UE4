#pragma once

#include "IceBreakers.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "EffectContainer.h"
#include "IBTargetType.generated.h"

class AIBCharacter;
class AActor;
struct FGameplayEventData;

UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class ICEBREAKERS_API UIBTargetType : public UObject
{
	GENERATED_BODY()

public:
	UIBTargetType() {}

	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AIBCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors);
};

UCLASS(NotBlueprintable)
class ICEBREAKERS_API UIBTargetType_UseOwner : public UIBTargetType
{
	GENERATED_BODY()

public:
	UIBTargetType_UseOwner() {}

	virtual void GetTargets_Implementation(AIBCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) override;
};

UCLASS(NotBlueprintable)
class ICEBREAKERS_API UIBTargetType_UseEventData : public UIBTargetType
{
	GENERATED_BODY()

public:
	UIBTargetType_UseEventData() {}

	virtual void GetTargets_Implementation(AIBCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) override;
};
