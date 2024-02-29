// Copyright Shawwy


#include "AbilitySystem/AbilityTask/TargetDataUnderCursor.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{

	UTargetDataUnderCursor* MyObj = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);


	return MyObj;
}

void UTargetDataUnderCursor::Activate()
{

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{ 
		// If we are on the server or client and locally controlled.
		SendMouseCursorData();
	}
	else
	{
		//TODO: We are on the server, so listen for target data
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey predictionKey = GetActivationPredictionKey();

		//Here, there are 2 scenarios:
		//1: The target data reach the server before the server run this line, then AbilityTargetDataSetDelegate() will not let
		// CallReplicatedTargetDataDelegatesIfSet() to be true, then it executes the if() syntax and 
		// notifies the ability that the server is still waiting for the data, forcing the client to send the TargetData again.
	
		
		//2: The target data reach the server after the server run this line, then once the data reaches, 
		// it will call the callback function immediatelly. And CallReplicatedTargetDataDelegatesIfSet is gonna be 'true'

		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, predictionKey).
			AddUObject(this,&UTargetDataUnderCursor::OnTargetDataReplicatedCallback);


		//If the target data has already reached to the server but the binding hasn't happend
		// 
		//If not, then notify the ability that server is still waiting for the data
		if (!AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, predictionKey))
			SetWaitingOnRemotePlayerData();


	}

}

void UTargetDataUnderCursor::SendMouseCursorData()
{

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	//Retrieve and pack the hit result target data
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	Data->HitResult = CursorHit;

	DataHandle.Add(Data);

	//Sending data to the server
	FGameplayTag ApplicationTag;

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);


	//Brocasting the data to the blueprint
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, 
	FGameplayTag ActivationTag)
{

	const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	const FPredictionKey predictionKey = GetActivationPredictionKey();

	//First, inform the ability system component that the data has been received
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(SpecHandle, predictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

