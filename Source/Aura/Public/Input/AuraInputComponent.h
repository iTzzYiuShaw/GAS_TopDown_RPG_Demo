// Copyright Shawwy

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, 
	PressedFuncType PressFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)

{
	check(InputConfig);

	for (auto InputAction : InputConfig->AbilittyInputActions)
	{
		if (InputAction.Value.InputAction && InputAction.Value.InputTag.IsValid())
		{

			if (PressFunc)
			{
				BindAction(InputAction.Value.InputAction, ETriggerEvent::Started, Object, PressFunc, InputAction.Value.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(InputAction.Value.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.Value.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(InputAction.Value.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.Value.InputTag);
			}

			
		}
	}
}
