#include "IBTargetType.h"
#include "Abilities/GameplayAbility.h"
#include "IBCharacter.h"

void UIBTargetType::GetTargets_Implementation(AIBCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors)
{
}

void UIBTargetType_UseOwner::GetTargets_Implementation(AIBCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors)
{
	OutActors.Add(TargetingCharacter);
}

void UIBTargetType_UseEventData::GetTargets_Implementation(AIBCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors)
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}