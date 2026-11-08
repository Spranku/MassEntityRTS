// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RTS_PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Core/RTS_BasePawn.h"

ARTS_PlayerController::ARTS_PlayerController()
{
	bShowMouseCursor = true;
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

		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Completed, this, &ARTS_PlayerController::CommandSelectedActor);
	}

}

void ARTS_PlayerController::Select(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SelectAction"));

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
		UE_LOG(LogTemp, Warning, TEXT("Selected Actor: %s"), *SelectedActor->GetName());

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
