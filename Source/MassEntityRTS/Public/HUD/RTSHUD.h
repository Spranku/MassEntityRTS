// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSHUD.generated.h"

/**
 * 
 */
UCLASS()
class MASSENTITYRTS_API ARTSHUD : public AHUD
{
	GENERATED_BODY()
	
private:

	bool bDrawSelectionRect = false;

	bool bSelectActos = false;

	FVector2D SelectionRectStart;

	FVector2D SelectionRectSize;

	UPROPERTY()
	TArray<AActor*> SelectedActors;

protected:

	virtual void DrawHUD() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Select color")
	FLinearColor SelectionRectColor = FLinearColor(1.0f, 0.0f, 0.2f);

public:

	void SelectActorsInRect();

	void ShowSelectionRect(const FVector2D inSelectionRectStart, const FVector2D InSelectionRectSize);

	void HideSelectionRect() { bDrawSelectionRect = false; bSelectActos = true; }

	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetSelectedActors() const { return SelectedActors; }
};
