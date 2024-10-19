// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "InputAction.h"

#include "GunPlayerController.generated.h"


class APlayerCharacter;

UCLASS()
class PROJECTFW_API AGunPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGunPlayerController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	// HUD 업데이트용
	UFUNCTION(BlueprintImplementableEvent)
	void ShowCrosshair(bool bShow);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMainHealthBar(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmoBar(float percent, FLinearColor Color);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlotHealth(int slot, float percent);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlotColor (int slot, FLinearColor Color);

	UFUNCTION(BlueprintPure)
	UUserWidget* GetGunHUD() const;

private:
	// Input Callback
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void Jump();
	void OnDash();
	void OffDash();
	void StartShoot();
	void ChangeMode();

	void UseSlot1();
	void UseSlot2();
	void UseSlot3();
	
	void ChangeCharacter(int index);
	void SetMode();
	// UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> GunHUDClass;
	UPROPERTY()
	class UUserWidget* GunHUD;

	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DashAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ChangeModeAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* UseSlot1Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* UseSlot2Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* UseSlot3Action;


	// Character
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<class APlayerCharacter>>  PlayerCharacterClasses;
	UPROPERTY()
	TArray<APlayerCharacter*>  PlayersInParty;

	bool bAttackMode = true;
	int CurrentPlayer = -1;
	FVector DummyLocation = FVector(-999.0f, -999.0f, -999.0f);
};
