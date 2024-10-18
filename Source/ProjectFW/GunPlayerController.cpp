// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"

AGunPlayerController::AGunPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunPlayerController::Tick(float DeltaTime)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		UpdateMainHealthBar(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
		UpdateSlotHealth(1, PlayerCharacter->GetHealth() / PlayerCharacter->GetMaxHealth());
		UpdateSlotColor(1, FLinearColor(1.0f, 0.0f, 0.0f) * 0.5f);
		UpdateAmmoBar(1.0f, FLinearColor(1.0f, 0.0f, 0.0f));

		UpdateSlotHealth(2, 0.0f);
		UpdateSlotHealth(3, 0.0f);
		UpdateSlotColor(2, FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		UpdateSlotColor(3, FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
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
}

void AGunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (ChangeModeAction)
		{
			Input->BindAction(ChangeModeAction, ETriggerEvent::Started, this, &AGunPlayerController::ChangeMode);
		}
	}
}

UUserWidget* AGunPlayerController::GetGunHUD() const
{
	return GunHUD;
}

void AGunPlayerController::ChangeMode()
{
	bAttackMode = !bAttackMode;
	SetMode();
}

void AGunPlayerController::SetMode()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->SetMode(bAttackMode);
	}
	ShowCrosshair(bAttackMode);
}
