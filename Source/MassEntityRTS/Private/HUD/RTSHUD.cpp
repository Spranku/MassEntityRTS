// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RTSHUD.h"

void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (bDrawSelectionRect)
	{
		DrawRect(SelectionRectColor, SelectionRectStart.X, SelectionRectStart.Y, SelectionRectSize.X, SelectionRectSize.Y);
	}
}

void ARTSHUD::ShowSelectionRect(const FVector2D inSelectionRectStart, const FVector2D InSelectionRectSize)
{
	SelectionRectStart = inSelectionRectStart;
	SelectionRectSize = InSelectionRectSize;
	bDrawSelectionRect = true;
}

