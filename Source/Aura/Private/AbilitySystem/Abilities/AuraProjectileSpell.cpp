// Copyright Shawwy


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/Combat_Interface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Here, we want the projectile is created and its data is maintained on the server, 
	// such as location, movement. The client side should only receive the replication data from the server
	

	

	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{

	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombat_Interface* CombatInterface = Cast<ICombat_Interface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		//Get location of socket weapon
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		//TODO: Get rotation
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give the Projectile a Gameplay Effect Spec for causing damage
		//Setting up a gameplayEffectSpec to the projectile
		//Projectile won't be spawned immediately for using SpawnActorDeferred
		//Loading necessary data for this damage gameplay effect
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		const FGameplayEffectSpecHandle ESH = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		float FinalDamage = Damage.GetValueAtLevel(5);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(ESH, GameplayTags.Damage, FinalDamage);

		Projectile->DamageEffectsSpecHandle = ESH;


		Projectile->FinishSpawning(SpawnTransform);
	}
}
