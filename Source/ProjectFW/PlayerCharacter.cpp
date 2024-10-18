// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gun.h"

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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, 5.0f));

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("gun_socket"));
		Gun->SetOwner(this);
	}
	ChangeMode();
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
		if (LookAction)
		{
			Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		if (DrawAction)
		{
			Input->BindAction(DrawAction, ETriggerEvent::Started, this, &APlayerCharacter::Draw);
		}
		if (DashAction)
		{
			Input->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::OnDash);
			Input->BindAction(DashAction, ETriggerEvent::Completed, this, &APlayerCharacter::OffDash);
		}
	}
}

void APlayerCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	FRotator Rotation = GetController()->GetControlRotation();
	// Use only yaw
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;

	FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Value.Y);
	AddMovementInput(RightDirection, Value.X);
}

void APlayerCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	AddControllerPitchInput(-Value.Y * CameraRotationWithMouseSpeed);
	AddControllerYawInput(Value.X * CameraRotationWithMouseSpeed);
}

void APlayerCharacter::Draw()
{
	bAttackMode = !bAttackMode;
	ChangeMode();
}

void APlayerCharacter::OnDash()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::OffDash()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::ChangeMode()
{
	if (bAttackMode)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		if (AttackModeAnimationBP)
		{
			GetMesh()->SetAnimInstanceClass(AttackModeAnimationBP);
			GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, 5.0f));
			
			if (SpringArm)
			{
				SpringArm->SetRelativeLocation(FVector(0.f, CameraOffsetInAttackMode, CameraHeight));
			}
		}
		if (Gun)
		{
			Gun->Draw(true);
		}
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (NormalModeAnimationBP)
		{
			GetMesh()->SetAnimInstanceClass(NormalModeAnimationBP);
			GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, -5.0f));

			if (SpringArm)
			{
				SpringArm->SetRelativeLocation(FVector(0.f, 0.f, CameraHeight));
			}
		}
		if (Gun)
		{
			Gun->Draw(false);
		}
	}
}
