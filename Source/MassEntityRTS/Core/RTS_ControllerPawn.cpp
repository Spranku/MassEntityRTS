// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RTS_ControllerPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ARTS_ControllerPawn::ARTS_ControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

// Called when the game starts or when spawned
void ARTS_ControllerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
}

void ARTS_ControllerPawn::Move(const FInputActionValue& Value)
{
	if (!IsValid(Controller)) return;

	const auto MovementInput = Value.Get<FVector2D>();

	const auto Rotation = Controller->GetControlRotation();
	const auto YawRotation = FRotator(0, Rotation.Yaw, 0);

	const auto Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MovementInput.Y);
	AddMovementInput(Right, MovementInput.X);

}

void ARTS_ControllerPawn::Zoom(const FInputActionValue& Value)
{
	if (!IsValid(Controller)) return;

	const auto ZoomDirection = Value.Get<float>();

	auto DesiredOrthiWidWidth = Camera->OrthoWidth - ZoomDirection * CameraZoomSpeed;
	DesiredOrthiWidWidth = FMath::Clamp(DesiredOrthiWidWidth, MinOrthoWidth, MaxOrthoWidth);
	Camera->OrthoWidth = DesiredOrthiWidWidth;
}

void ARTS_ControllerPawn::EdgeScroll()
{
	if (!PlayerController) return;

	float MouseX = 0.0f, MouseY = 0.0f;
	if (PlayerController->GetMousePosition(MouseX, MouseY))
	{
		FVector2D ViewportSize;
		FVector2D MovementInput = FVector2D::ZeroVector;

		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);

			constexpr float EDGE_TRESHLODS = 10.0f;

			if (MouseX < EDGE_TRESHLODS)
			{
				MovementInput.X = -1.f;
			}
			if (MouseX > (ViewportSize.X - EDGE_TRESHLODS))
			{
				MovementInput.X = 1.f;
			}
			if (MouseY < EDGE_TRESHLODS)
			{
				MovementInput.Y = 1.f;
			}
			if (MouseY > (ViewportSize.Y - EDGE_TRESHLODS))
			{
				MovementInput.Y = -1.f;
			}

			if (!MovementInput.IsZero())
			{
				Move(FInputActionValue(MovementInput));
			}
		}
	}
}

// Called every frame
void ARTS_ControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EdgeScroll();
}

// Called to bind functionality to input
void ARTS_ControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARTS_ControllerPawn::Move);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARTS_ControllerPawn::Zoom);
	}
}

