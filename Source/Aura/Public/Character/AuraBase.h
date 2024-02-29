// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/Combat_Interface.h"
#include "AbilitySystemComponent.h"
#include "AuraBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
UCLASS()
class AURA_API AAuraBase : public ACharacter , public IAbilitySystemInterface, public ICombat_Interface
{
	GENERATED_BODY()

public:
	AAuraBase();

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet;}

protected:
	virtual void BeginPlay() override;

	//Weapon is detached from the chracter and dropped on the ground
	//TODO:Enemy: Ragdoll, 
	//TODO:Character: Montage
	virtual void Die() override;

	//For Enemey: Ragdoll needs to be done on server and the client.
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	//Name for searching weapon socket
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;

	/** Here, the ability system component and attributeSet is for the enemy class
		Beacuse it will not own a playerState
		
		For the playerControlled character, these two are handled by a PlayerState,
		which handles their initialization and Getter*/

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;


	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const;

	virtual void InitializeDefaultAttributes() const;
	
	
	//Will be called on Ability system component which grands start up abilities
	void AddCharacterAbilities();

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;


	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
