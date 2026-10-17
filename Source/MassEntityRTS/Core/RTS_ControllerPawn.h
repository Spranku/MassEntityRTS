// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTS_ControllerPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UFloatingPawnMovement;

UCLASS()
class MASSENTITYRTS_API ARTS_ControllerPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

public:	
	// Sets default values for this pawn's properties
	ARTS_ControllerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
