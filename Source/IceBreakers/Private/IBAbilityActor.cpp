// Fill out your copyright notice in the Description page of Project Settings.


#include "IBAbilityActor.h"

// Sets default values
AIBAbilityActor::AIBAbilityActor()
	: EffectContainerSpec{},
	HitActors{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIBAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIBAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
