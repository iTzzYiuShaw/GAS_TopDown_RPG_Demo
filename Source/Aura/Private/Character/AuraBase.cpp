// Copyright Shawwy


#include "Character/AuraBase.h"


//Base class for any character
AAuraBase::AAuraBase()
{
	PrimaryActorTick.bCanEverTick = false;


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


