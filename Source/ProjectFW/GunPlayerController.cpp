// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"
#include "StatComponent.h"

AGunPlayerController::AGunPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCharacterClasses.Init(nullptr, 3);
	PlayerCharacters.Init(nullptr, 3);
}

void AGunPlayerController::Tick(float DeltaTime)
{
	if (!PlayerCharacters[0])
	{
		PlayerCharacters[0] = Cast<APlayerCharacter>(GetPawn());
	}

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		UpdateMainHealthBar(PlayerCharacter->GetStat()->GetHealth(), PlayerCharacter->GetStat()->GetMaxHealth());
		UpdateAmmoBar(1.0f, PlayerCharacter->GetStat()->GetAttributeColor());

		for (int i = 0; i < PlayerCharacters.Num(); ++i)
		{
			if (PlayerCharacters[i])
			{
				UpdateSlotHealth(i, PlayerCharacters[i]->GetStat()->GetHealth() / PlayerCharacters[i]->GetStat()->GetMaxHealth());
				UpdateSlotColor(i, PlayerCharacters[i]->GetStat()->GetAttributeColor() * 0.5f);
			}
		}
	}
}

void AGunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	GunHUD = CreateWidget(this, GunHUDClass);
	if (GunHUD)
	{
		GunHUD->AddToViewport();
	}

	for (int i = 0; i < PlayerCharacterClasses.Num() -1; ++i)
	{
		if (PlayerCharacterClasses[i + 1])
		{
			PlayerCharacters[i + 1] = GetWorld()->SpawnActor<APlayerCharacter>(
				PlayerCharacterClasses[i + 1], FVector(300.0f + 30.0f* i, 1700.0f, 50.0f), FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AGunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGunPlayerController::Move);
		}
		if (JumpAction)
		{
			Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGunPlayerController::Jump);
		}
		if (LookAction)
		{
			Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGunPlayerController::Look);
		}
		if (DashAction)
		{
			Input->BindAction(DashAction, ETriggerEvent::Started, this, &AGunPlayerController::OnDash);
			Input->BindAction(DashAction, ETriggerEvent::Completed, this, &AGunPlayerController::OffDash);
		}
		if (ShootAction)
		{
			Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGunPlayerController::StartShoot);
		}
		if (ChangeModeAction)
		{
			Input->BindAction(ChangeModeAction, ETriggerEvent::Started, this, &AGunPlayerController::ChangeMode);
		}
		if (UseSlot1Action)
		{
			Input->BindAction(UseSlot1Action, ETriggerEvent::Started, this, &AGunPlayerController::UseSlot1);
		}
		if (UseSlot2Action)
		{
			Input->BindAction(UseSlot2Action, ETriggerEvent::Started, this, &AGunPlayerController::UseSlot2);
		}
		if (UseSlot3Action)
		{
			Input->BindAction(UseSlot3Action, ETriggerEvent::Started, this, &AGunPlayerController::UseSlot3);
		}
	}
}

UUserWidget* AGunPlayerController::GetGunHUD() const
{
	return GunHUD;
}

void AGunPlayerController::Move(const FInputActionInstance& Instance)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		FVector2D Vector = Instance.GetValue().Get<FVector2D>();
		PlayerCharacter->Move(Vector);
	}
}

void AGunPlayerController::Look(const FInputActionInstance& Instance)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		FVector2D Vector = Instance.GetValue().Get<FVector2D>();
		PlayerCharacter->Look(Vector);
	}
}

void AGunPlayerController::Jump()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Jump();
	}
}

void AGunPlayerController::OnDash()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->OnDash();
	}
}

void AGunPlayerController::OffDash()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->OffDash();
	}
}

void AGunPlayerController::StartShoot()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->StartShoot();
	}
}

void AGunPlayerController::ChangeMode()
{
	bAttackMode = !bAttackMode;
	SetMode();
}

void AGunPlayerController::UseSlot1()
{
	ChangeCharacter(0);
}

void AGunPlayerController::UseSlot2()
{
	ChangeCharacter(1);
}

void AGunPlayerController::UseSlot3()
{
	ChangeCharacter(2);
}

void AGunPlayerController::ChangeCharacter(int index)
{
	if (CurrentPlayer == index)
	{
		return;
	}

	if (PlayerCharacters.Num() > index)
	{
		if (PlayerCharacters[index])
		{
			FVector Location = GetPawn()->GetActorLocation();
			FRotator Rotation = GetPawn()->GetActorRotation();

			PlayerCharacters[index]->SetActorLocation(GetPawn()->GetActorLocation());
			PlayerCharacters[index]->SetActorRotation(Rotation);

			UnPossess();
			Possess(PlayerCharacters[index]);

			CurrentPlayer = index;

			SetMode();
		}
	}
}

void AGunPlayerController::SetMode()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->SetMode(bAttackMode);
	}
	ShowCrosshair(bAttackMode);
}
