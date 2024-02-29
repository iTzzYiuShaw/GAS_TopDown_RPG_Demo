// Copyright Shawwy


#include "Character/AuraBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Aura/Aura.h"
//Base class for any character
AAuraBase::AAuraBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuraBase::MulticastHandleDeath()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);


	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector AAuraBase::GetCombatSocketLocation()
{


	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraBase::InitAbilityActorInfo()
{
}

/*
Can be used to initialize default primary/secondary attributes of this character,
depending on different gameplay effect class, which is designed and assgined in blueprint side
*/

void AAuraBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const
{

	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraBase::InitializeDefaultAttributes() const
{
	//Initialize primary attributes
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);

	//Initialize secondary attributes
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);

	//Initialize Vital attributes
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraBase::AddCharacterAbilities()
{
	
	UAuraAbilitySystemComponent* ASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
}

UAnimMontage* AAuraBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}



