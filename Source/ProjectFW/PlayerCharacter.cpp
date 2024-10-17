// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem 
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
		if (JumpAction)
		{
			Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		}
		if (CameraRotateAction)
		{
			Input->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RotateCamera);
		}
	}
}

void APlayerCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	AddMovementInput(GetActorForwardVector() * Value.Y + GetActorRightVector() * Value.X);
}

void APlayerCharacter::RotateCamera(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	AddControllerPitchInput(-Value.Y * CameraRotationWithMouseSpeed);
	AddControllerYawInput(Value.X * CameraRotationWithMouseSpeed);
}

