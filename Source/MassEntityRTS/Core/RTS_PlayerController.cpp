// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RTS_PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Core/RTS_BasePawn.h"
#include "HUD/RTSHUD.h"

ARTS_PlayerController::ARTS_PlayerController()
{
	bShowMouseCursor = true;
}

void ARTS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	RTSHUD = Cast<ARTSHUD>(GetHUD());

}

void ARTS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!DefaultInputMappingContext) return;

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("Success add maxipng context"));
	}

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ARTS_PlayerController::Select);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ARTS_PlayerController::SelectStart);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ARTS_PlayerController::SelectOnGoing);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ARTS_PlayerController::SelectEnd);


		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Completed, this, &ARTS_PlayerController::CommandSelectedActor);
	}

}

void ARTS_PlayerController::Select(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("SelectAction"));

	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);

	// Deselect previously selected actor
	if (SelectedActor)
	{
		if (SelectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(SelectedActor, false);
		}
	}

	SelectedActor = HitResult.GetActor();

	if (SelectedActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Selected Actor: %s"), *SelectedActor->GetName());

		if (SelectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(SelectedActor, true);
		}
	}
}

void ARTS_PlayerController::CommandSelectedActor(const FInputActionValue& Value)
{
	if (SelectedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("CommandAction: %s"), *SelectedActor->GetName());

		if (SelectedActor->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
		{
			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);

			if (HitResult.bBlockingHit)
			{
				INavigableInterface::Execute_MoveToLocation(SelectedActor, HitResult.Location);
			}
		}
	}
}

void ARTS_PlayerController::SelectStart(const FInputActionValue& Value)
{
	float MouseX, MouseY;
	GetMousePosition(MouseX, MouseY);

	SelectionStartPosition = FVector2D(MouseX, MouseY);
	UE_LOG(LogTemp, Display, TEXT("Selection started"));
}

void ARTS_PlayerController::SelectOnGoing(const FInputActionValue& Value)
{
	float MouseX, MouseY;
	GetMousePosition(MouseX, MouseY);
	SelectionSize = FVector2D(MouseX - SelectionStartPosition.X, MouseY - SelectionStartPosition.Y);
	//UE_LOG(LogTemp, Warning, TEXT("SelectOnGoing"));
	if (RTSHUD)
	{
		RTSHUD->ShowSelectionRect(SelectionStartPosition, SelectionSize);
	}
}

void ARTS_PlayerController::SelectEnd(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Select End"));
	constexpr float DELAY_TIME = 0.2f;

	if (!RTSHUD) return;
	RTSHUD->HideSelectionRect();

	FTimerHandle TimerHandle;
	auto World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &ARTS_PlayerController::SelectMultiplyActors, DELAY_TIME, false);
	}

}

void ARTS_PlayerController::SelectMultiplyActors()
{
	if (!RTSHUD) return;

	/* Deselect old selected actors */
	for (auto* i : SelectedActors)
	{
		if (i && i->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(i, false);
		}
	}

	/* Select new actors */
	RTSHUD->HideSelectionRect();
	SelectedActors = RTSHUD->GetSelectedActors();

	for (auto* i : SelectedActors)
	{
		if (i && i->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(i, true);
		}
	}
}
