// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "StatComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		if (HealthBarClass)
		{
			HealthBar = CreateWidget(PlayerController, HealthBarClass);
		}	
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthBar)
	{
		UpdateHealthBarLocation();
	}
}

UUserWidget* AEnemyAIController::GetHealthBar() const
{
	return HealthBar;
}

void AEnemyAIController::UpdateHealthBarLocation()
{
	if (!PlayerController)
	{
		return;
	}

	FVector Location = GetPawn()->GetActorLocation();
	Location.Z += HealthBarHeight;

	
	if (APawn* PlayerPawn = PlayerController->GetPawn())
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		if (FVector::Distance(PlayerLocation, Location) <= HealthBarVisibleDistance)
		{
			if (!HealthBar->IsInViewport())
			{
				HealthBar->AddToViewport(1);
			}
			FVector2D ScreenLocation;
			PlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);

			HealthBar->SetPositionInViewport(ScreenLocation);

			if (APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetPawn()))
			{
				if (UStatComponent* Stat = OwningCharacter->GetStat())
				{
					UpdateHealthBar(Stat->GetHealthRate(), Stat->GetLevel());
				}
			}
		}
		else
		{
			if (HealthBar->IsInViewport())
			{
				HealthBar->RemoveFromViewport();
			}
		}
	}
}
