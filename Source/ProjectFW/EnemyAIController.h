// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFW_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthBar(float percent, int level);

	UFUNCTION(BlueprintPure)
	UUserWidget* GetHealthBar() const;

private:
	void UpdateHealthBarLocation();

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> HealthBarClass;
	UPROPERTY()
	class UUserWidget* HealthBar;

	UPROPERTY(EditAnywhere, Category = "Widget")
	float HealthBarHeight = 110.0f;
	UPROPERTY(EditAnywhere, Category = "Widget")
	float HealthBarVisibleDistance = 500.0f;

	APlayerController* PlayerController; 
};
