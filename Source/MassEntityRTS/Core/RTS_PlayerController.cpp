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
	}

}

void ARTS_PlayerController::Select(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SelectAction"));

	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);

	auto* SelectedActor = HitResult.GetActor();

	if (SelectedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Actor: %s"), *SelectedActor->GetName());

		/* Deselect previous actor */ 
		if (SelectedPawn) { SelectedPawn->SelectActor(false); }

		/* Select new actor */
		if (SelectedPawn = Cast<ARTS_BasePawn>(SelectedActor)) { SelectedPawn->SelectActor(true); }
	}
}
