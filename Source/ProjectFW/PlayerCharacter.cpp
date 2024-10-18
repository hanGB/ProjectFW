// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("gun_socket"));
		Gun->SetOwner(this);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	SetMode(true);

	Health = MaxHealth;
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
}

float APlayerCharacter::GetBlendWeightForMovement() const
{
	if (bShooting && GetCharacterMovement()->MaxWalkSpeed == RunSpeed)
	{
		return 1.0f;
	}
	else
	{
		return 0.0f;
	}
}

bool APlayerCharacter::IsDead() const
{
	return Health <= 0.0f;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageCauser->GetOwner() != this)
	{
		DamageToApply = FMath::Min(DamageToApply, Health);
		Health -= DamageToApply;
	}

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	return DamageToApply;
}

void APlayerCharacter::Move(FVector2D Vector)
{
	FRotator Rotation = GetController()->GetControlRotation();
	// Use only yaw
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;

	FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Vector.Y);
	AddMovementInput(RightDirection, Vector.X);
}

void APlayerCharacter::Look(FVector2D Vector)
{
	AddControllerPitchInput(-Vector.Y * CameraRotationWithMouseSpeed);
	AddControllerYawInput(Vector.X * CameraRotationWithMouseSpeed);
}

void APlayerCharacter::OnDash()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::OffDash()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::StartShoot()
{
	if (bShooting)
	{
		return;
	}

	// Only Shoot in Attack Mode
	if (Gun)
	{
		if (Gun->IsDrawed())
		{
			Gun->PullTrigger();
			if (FireAnimMontage)
			{
				PlayAnimMontage(FireAnimMontage);
			}
			bShooting = true;
		}
	}
	
}

void APlayerCharacter::EndShoot()
{
	// Call function by AnimNotify End Fire 
	bShooting = false;
}

void APlayerCharacter::SetMode(bool bAttackMode)
{
	if (bAttackMode)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		if (AttackModeAnimationBP)
		{
			GetMesh()->SetAnimInstanceClass(AttackModeAnimationBP);
			FVector MeshLocation = GetMesh()->GetRelativeLocation();
			GetMesh()->SetRelativeLocation(FVector(MeshLocation.X, MeshLocation.Y, AttackModeHeight));
			if (SpringArm)
			{
				SpringArm->SetRelativeLocation(FVector(0.f, CameraOffsetInAttackMode, CameraHeight));
			}
		}
		if (Gun)
		{
			Gun->Draw(true);
			bShooting = false;
		}
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (NormalModeAnimationBP)
		{
			GetMesh()->SetAnimInstanceClass(NormalModeAnimationBP);
			FVector MeshLocation = GetMesh()->GetRelativeLocation();
			GetMesh()->SetRelativeLocation(FVector(MeshLocation.X, MeshLocation.Y, NormalModeHeight));
			if (SpringArm)
			{
				SpringArm->SetRelativeLocation(FVector(0.f, 0.f, CameraHeight));
			}
		}
		if (Gun)
		{
			Gun->Draw(false);
			bShooting = false;
		}
	}
}

float APlayerCharacter::GetHealth() const
{
	return Health;
}

float APlayerCharacter::GetMaxHealth() const
{
	return MaxHealth;
}
