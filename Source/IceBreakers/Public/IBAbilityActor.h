// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IceBreakers.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "EffectContainer.h"
#include "IBAbilityActor.generated.h"

UCLASS()
class ICEBREAKERS_API AIBAbilityActor : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FIBGameplayEffectContainerSpec EffectContainerSpec;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	TArray<AActor*> HitActors;

public:	
	// Sets default values for this actor's properties
	AIBAbilityActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
