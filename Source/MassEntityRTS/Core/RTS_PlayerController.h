// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ARTS_BasePawn;

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

	UPROPERTY()
	TObjectPtr<ARTS_BasePawn> SelectedPawn = nullptr;

protected:

	virtual void SetupInputComponent() override;

	void Select(const FInputActionValue& Value);
};
