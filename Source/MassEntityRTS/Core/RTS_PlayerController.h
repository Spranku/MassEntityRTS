// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ARTS_BasePawn;
class ARTSHUD;

UCLASS()
class MASSENTITYRTS_API ARTS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARTS_PlayerController();

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CommandAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARTSHUD> RTSHUD = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> SelectedActor = nullptr;

	UPROPERTY()
	TArray<AActor*> SelectedActors;

	/* Actor selected variables */
	FVector2D SelectionStartPosition = FVector2D(0);

	FVector2D SelectionSize = FVector2D(0);

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Select(const FInputActionValue& Value);

	void CommandSelectedActors(const FInputActionValue& Value);

	/* Actors selected */
	void SelectStart(const FInputActionValue& Value);

	void SelectOnGoing(const FInputActionValue& Value);

	void SelectEnd(const FInputActionValue& Value);

	void SelectMultiplyActors();
};
