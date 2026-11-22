// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RTS_BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

// Sets default values
ARTS_BasePawn::ARTS_BasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	RootComponent = CapsuleComponent;

	SelectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedIndicator"));
	SelectedIndicator->SetupAttachment(RootComponent);
	SelectedIndicator->SetHiddenInGame(true);
	SelectedIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

}

// Called when the game starts or when spawned
void ARTS_BasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTS_BasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OrientPawnToMovement();
}

void ARTS_BasePawn::OrientPawnToMovement()
{
	if (!bMoving) return;

	FVector MoveDirection = (MoveTargetLocation - GetActorLocation());
	if (MoveDirection.Length() < AcceptanceDistance)
	{
		bMoving = false;
		return;
	}

	MoveDirection.Normalize(1);
	FRotator DesiredRotation = UKismetMathLibrary::MakeRotFromX(MoveDirection);
	DesiredRotation.Pitch = 0;
	DesiredRotation.Roll = 0;

	FRotator NewRotator = FMath::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(), CharacterTurnSpeed);
	SetActorRotation(NewRotator);
}

// Called to bind functionality to input
void ARTS_BasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARTS_BasePawn::SelectActor_Implementation(const bool bSelect)
{
	SelectedIndicator->SetHiddenInGame(!bSelect);
}

void ARTS_BasePawn::MoveToLocation_Implementation(const FVector TargetLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Navigation"));
	MoveTargetLocation = TargetLocation + FVector(0, 0, GetDefaultHalfHeight());
	bMoving = true;

	auto AIController = Cast<AAIController>(GetController());
	if (!AIController) return;
	AIController->MoveToLocation(TargetLocation, AcceptanceDistance);
}

void ARTS_BasePawn::DeselectActor()
{
}

