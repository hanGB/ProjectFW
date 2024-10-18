// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GunPlayerController.generated.h"

/**
 * 
 */
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
	void ChangeMode();

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
	class UInputAction* ChangeModeAction;

	bool bAttackMode = true;
};
