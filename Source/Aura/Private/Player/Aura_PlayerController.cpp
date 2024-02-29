// Copyright Shawwy


#include "Player/Aura_PlayerController.h"
#include "Interaction/Enemy_Interface.h"
#include "Input/AuraInputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitysystemLibrary.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

AAura_PlayerController::AAura_PlayerController()
{

	//Replication
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAura_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}


void AAura_PlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}




void AAura_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Check if it's valid
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	//Subsystem is not valid unless it's on local machine
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAura_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAura_PlayerController::Move);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);

	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAura_PlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAura_PlayerController::ShiftReleased);
}

void AAura_PlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAura_PlayerController::CursorTrace()
{

	/*
		Line trace from cursor. There are several scenarios:
		A. LastActor is null && ThisActor is null
			- Do nothing

		B. LastActor is null && ThisAcotr is valid
			- Highlight this ThisActor

		C. LastActor is valid && ThisActor is null
			- UnHighlight LastActor

		D. Both are valid, but LastActor != ThisActor
			- UnHighlight lastActor && Highlight thisActor

		E. D. Both are valid, and LastActor = ThisActor
			- Do nothing
	*/

	//Raytracing, from camera to cursor
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (CursorHit.bBlockingHit)
	{
		LastActor = ThisActor;
		ThisActor = Cast<IEnemy_Interface>(CursorHit.GetActor());

		//if (LastActor == nullptr)
		//{
		//	if (ThisActor != nullptr)
		//	{
		//		// CaseB
		//		ThisActor->HightlightActor();
		//	}
		//	//Case A do nothing
		//}
		//else
		//{
		//	if (ThisActor == nullptr)
		//	{
		//		LastActor->UnHighlightActor();
		//	}
		//	else
		//	{
		//		if (LastActor != ThisActor)
		//		{
		//			LastActor->UnHighlightActor();
		//			ThisActor->HightlightActor();
		//		}
		//		//Case E 
		//	}
		//}


		if (LastActor != ThisActor)
		{
			if (LastActor) LastActor->UnHighlightActor();
			if (ThisActor) ThisActor->HightlightActor();
		}
	}


}

void AAura_PlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTartgeting = ThisActor ? true : false;
		bAutoRunning = false;
	}


}

void AAura_PlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInfoTagReleased(InputTag);

		return;
	}

	if (GetASC()) GetASC()->AbilityInfoTagReleased(InputTag);

	if (!bTartgeting && !bShiftKeyDown)
	{
		//Click to move
		//Check if this is a short press

		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{

			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}


		}
		FollowTime = 0.f;
		bTartgeting = false;
		
	}
}

void AAura_PlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	//If this is not left mouse button, we want to try to activate the ability
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInfoTagHeld(InputTag);
		return;
	}
	
	if (bTartgeting || bShiftKeyDown)
	{
		if (GetASC()) GetASC()->AbilityInfoTagHeld(InputTag);
		
	}
	else
	{
		//Click to move 

		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;


		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}


}

UAuraAbilitySystemComponent* AAura_PlayerController::GetASC()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>
			(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AbilitySystemComponent;
}


