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

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowCrosshair(bool bShow);

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
