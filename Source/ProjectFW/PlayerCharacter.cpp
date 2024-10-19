// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gun.h"
#include "StatComponent.h"
#include "FloatingDamageText.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	Camera->SetupAttachment(SpringArm);

	Stat = CreateDefaultSubobject<UStatComponent>(TEXT("Stat"));
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
	return Stat->IsDead();
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageCauser->GetOwner() != this)
	{
		EAttribute Attribute = EAttribute::Non;
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DamageCauser->GetOwner()))
		{
			Attribute = PlayerCharacter->GetStat()->GetAttribute();
			DamageToApply = PlayerCharacter->GetStat()->CaculateDamageCaused(DamageToApply);
		}

		DamageToApply = Stat->CaculateDamageReceivedAndApply(DamageToApply, Attribute);

		// 데미지 텍스트 스폰
		if (FloatingDamageTextClass)
		{
			FHitResult HitResult;
			FVector Direction;
			DamageEvent.GetBestHitInfo(this, EventInstigator, HitResult, Direction);

			FVector Location = HitResult.Location + FVector(FMath::RandRange(-30.0f, 30.0f), FMath::RandRange(-30.0f, 30.0f), FMath::RandRange(-30.0f, 30.0f));
			FRotator Rotation = Direction.Rotation();

			AFloatingDamageText* FloatingDamageText = GetWorld()->SpawnActor<AFloatingDamageText>(FloatingDamageTextClass, Location, Rotation);
			if (FloatingDamageText)
			{
				FloatingDamageText->SetDamageText(DamageToApply, UStatComponent::AttributeColor(Attribute));
			}
		}		
	}

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
			bool bShooted = Gun->PullTrigger();
			if (bShooted)
			{
				if (FireAnimMontage)
				{
					PlayAnimMontage(FireAnimMontage);
				}
				bShooting = true;
			}		
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

UStatComponent* APlayerCharacter::GetStat() const
{
	return Stat;
}

float APlayerCharacter::GetAmmoRate() const
{
	if (Gun)
	{
		return Gun->GetAmmoRate();
	}
	return 0.0f;
}

bool APlayerCharacter::GetIsReloading() const
{
	if (Gun)
	{
		return Gun->IsReloading();
	}
	return false;
}

void APlayerCharacter::BeginRest()
{
	if (Gun)
	{
		Gun->ResetRestTimer();
	}
}

void APlayerCharacter::Rest(float DeltaTime)
{
	if (Gun)
	{
		Gun->AddAmmoWhenRest(DeltaTime);
	}
}
