// Copyright Shawwy


#include "Player/Aura_PlayerController.h"
#include "Interaction/Enemy_Interface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAura_PlayerController::AAura_PlayerController()
{

	//Replication
	bReplicates = true;
}

void AAura_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAura_PlayerController::Move);
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

	FHitResult CursorHit;

	//Raytracing, from camera to cursor
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (CursorHit.bBlockingHit)
	{
		LastActor = ThisActor;
		ThisActor = Cast<IEnemy_Interface>(CursorHit.GetActor());

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

		if (LastActor == nullptr)
		{
			if (ThisActor != nullptr)
			{
				// CaseB
				ThisActor->HightlightActor();
			}
			//Case A do nothing
		}
		else
		{
			if (ThisActor == nullptr)
			{
				LastActor->UnHighlightActor();
			}
			else
			{
				if (LastActor != ThisActor)
				{
					LastActor->UnHighlightActor();
					ThisActor->HightlightActor();
				}
				//Case E 
			}
		}


	}
}
