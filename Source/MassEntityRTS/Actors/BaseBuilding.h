// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SelectableInterface.h"
#include "BaseBuilding.generated.h"

class UStaticMeshComponent;

UCLASS()
class MASSENTITYRTS_API ABaseBuilding : public AActor, public ISelectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuilding();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SelectActor_Implementation(const bool bSelect) override;

private:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> SelectedIndicator;

};
