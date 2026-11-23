// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RTSHUD.h"

void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (bDrawSelectionRect)
	{
		DrawRect(SelectionRectColor, 
				 SelectionRectStart.X, 
				 SelectionRectStart.Y, 
				 SelectionRectSize.X, 
				 SelectionRectSize.Y);
	}

	if (bSelectActos) SelectActorsInRect();
}

void ARTSHUD::SelectActorsInRect()
{
	FVector2D FirstPoint = SelectionRectStart;
	FVector2D SecondPoint = SelectionRectStart + SelectionRectSize;

	GetActorsInSelectionRectangle<AActor>(FirstPoint, SecondPoint, SelectedActors, false);
	for (auto* i : SelectedActors)
	{
		UE_LOG(LogTemp, Error, TEXT("ActorsInRectangle: %s"), *i->GetName());
	}

	bSelectActos = false;
}

void ARTSHUD::ShowSelectionRect(const FVector2D inSelectionRectStart, const FVector2D InSelectionRectSize)
{
	SelectedActors.Empty();
	SelectionRectStart = inSelectionRectStart;
	SelectionRectSize = InSelectionRectSize;
	bDrawSelectionRect = true;
}

