#include "IBCharacter.h"

// Sets default values
AIBCharacter::AIBCharacter()
	: CharacterLevel{ 1 },
	bAbilitiesInitialized{ false },
	BaseMovementSpeed{ 600 }
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UIBAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AIBCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddStartupGameplayAbilities();
}

void AIBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AIBCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AIBCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void AIBCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIBCharacter, CharacterLevel);
}

// Called every frame
void AIBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AIBCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AIBCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AIBCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AIBCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float AIBCharacter::GetAttackPower() const
{
	return AttributeSet->GetAttackPower();
}

float AIBCharacter::GetAttackSpeed() const
{
	return AttributeSet->GetAttackSpeed();
}

float AIBCharacter::GetDefensePower() const
{
	return AttributeSet->GetDefensePower();
}

float AIBCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float AIBCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

float AIBCharacter::GetCooldownSpeed() const
{
	return 0.0f;
}

int32 AIBCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool AIBCharacter::SetCharacterLevel(int32 NewLevel)
{
	if (NewLevel > 0 && CharacterLevel != NewLevel)
	{
		CharacterLevel = NewLevel;

		return true;
	}
	return false;
}

bool AIBCharacter::ActivateAbilitiesWithKey(FKey Key, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpecHandle* FoundHandle = KeyboundAbilities.Find(Key);

	if (FoundHandle && AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbility(*FoundHandle, bAllowRemoteActivation);
	}

	return false;
}

void AIBCharacter::GetActiveAbilitiesWithKey(FKey Key, TArray<UGameplayAbility*>& ActiveAbilities)
{
	FGameplayAbilitySpecHandle* FoundHandle = KeyboundAbilities.Find(Key);

	if (FoundHandle && AbilitySystemComponent)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(*FoundHandle);

		if (FoundSpec)
		{
			TArray<UGameplayAbility*> AbilityInstances = FoundSpec->GetAbilityInstances();

			// Find all ability instances executed from this slot
			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}
}

bool AIBCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

void AIBCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

		// Iterate the list of all ability specs
		for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
		{
			// Iterate all instances on this ability spec
			TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}
}

bool AIBCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

void AIBCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, AIBCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void AIBCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::HandleManaChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnManaChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::HandleAttackPowerChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnAttackPowerChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::HandleAttackSpeedChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnAttackSpeedChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::HandleDefensePowerChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnDefensePowerChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::HandleMoveSpeedChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed * GetMoveSpeed();

	if (bAbilitiesInitialized)
	{
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::HandleCooldownSpeedChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnCooldownSpeedChanged(DeltaValue, EventTags);
	}
}

void AIBCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		for (TSubclassOf<UGameplayAbility>& StartupAbility : InitialGameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
		}

		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}
	AddKeyboundGameplayAbilities();

	bAbilitiesInitialized = true;
}

void AIBCharacter::RemoveStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && bAbilitiesInitialized)
	{
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && InitialGameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}

		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		AbilitySystemComponent->RemoveActiveEffects(Query);

		RemoveKeyboundGameplayAbilities(true);

		bAbilitiesInitialized = false;
	}
}

void AIBCharacter::AddKeyboundGameplayAbilities()
{
	TMap<FKey, FGameplayAbilitySpec> KeyboundAbilitySpecs;
	FillKeyboundAbilitySpecs(KeyboundAbilitySpecs);

	// Now add abilities if needed
	for (const TPair<FKey, FGameplayAbilitySpec>& SpecPair : KeyboundAbilitySpecs)
	{
		FGameplayAbilitySpecHandle& SpecHandle = KeyboundAbilities.FindOrAdd(SpecPair.Key);

		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
		}
	}
}

void AIBCharacter::FillKeyboundAbilitySpecs(TMap<FKey, FGameplayAbilitySpec>& KeyboundAbilitySpecs)
{
	for (const TPair<FKey, TSubclassOf<UGameplayAbility>>& DefaultPair : DefaultKeyboundAbilities)
	{
		if (DefaultPair.Value.Get())
		{
			KeyboundAbilitySpecs.Add(DefaultPair.Key, FGameplayAbilitySpec(DefaultPair.Value, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void AIBCharacter::RemoveKeyboundGameplayAbilities(bool bRemoveAll)
{
	TMap<FKey, FGameplayAbilitySpec> KeyboundAbilitySpecs;

	if (!bRemoveAll)
	{
		// Fill in map so we can compare
		FillKeyboundAbilitySpecs(KeyboundAbilitySpecs);
	}

	for (TPair<FKey, FGameplayAbilitySpecHandle>& ExistingPair : KeyboundAbilities)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(ExistingPair.Value);
		bool bShouldRemove = bRemoveAll || !FoundSpec;

		if (!bShouldRemove)
		{
			// Need to check desired ability specs, if we got here FoundSpec is valid
			FGameplayAbilitySpec* DesiredSpec = KeyboundAbilitySpecs.Find(ExistingPair.Key);

			if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
			{
				bShouldRemove = true;
			}
		}

		if (bShouldRemove)
		{
			if (FoundSpec)
			{
				// Need to remove registered ability
				AbilitySystemComponent->ClearAbility(ExistingPair.Value);
			}

			// Make sure handle is cleared even if ability wasn't found
			ExistingPair.Value = FGameplayAbilitySpecHandle();
		}
	}
}
