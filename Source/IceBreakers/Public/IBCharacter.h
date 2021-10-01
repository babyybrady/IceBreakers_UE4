#pragma once

#include "IceBreakers.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "IBAttributeSet.h"
#include "IBCharacter.generated.h"

UCLASS()
class ICEBREAKERS_API AIBCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIBCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = Abilities)
	float BaseMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UIBAttributeSet* AttributeSet;

	UPROPERTY()
	int32 bAbilitiesInitialized;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TMap<FKey, TSubclassOf<UGameplayAbility>> DefaultKeyboundAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FKey, FGameplayAbilitySpecHandle> KeyboundAbilities;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetAttackPower() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetAttackSpeed() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetDefensePower() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetCooldownSpeed() const;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool ActivateAbilitiesWithKey(FKey Key, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = Abilities)
	void GetActiveAbilitiesWithKey(FKey Key, TArray<UGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = Abilities)
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AIBCharacter* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackPowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDefensePowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCooldownSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	void AddStartupGameplayAbilities();
	void RemoveStartupGameplayAbilities();
	void AddKeyboundGameplayAbilities();
	void FillKeyboundAbilitySpecs(TMap<FKey, FGameplayAbilitySpec>& KeyboundAbilitySpecs);
	void RemoveKeyboundGameplayAbilities(bool bRemoveAll);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AIBCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleAttackPowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleAttackSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleDefensePowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleCooldownSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
		
	friend UIBAttributeSet;
};
