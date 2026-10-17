// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_PlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class MASSENTITYRTS_API ARTS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARTS_PlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

protected:

	virtual void SetupInputComponent() override;
};
