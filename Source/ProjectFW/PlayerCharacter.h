// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTFW_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Input Callback
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void Draw();
	void OnDash();
	void OffDash();

	void ChangeMode();

	// Camera
	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraHeight = 55.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraOffsetInAttackMode = 40.0f;

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
	UInputAction* DrawAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	float CameraRotationWithMouseSpeed = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 400.0f;

	// Animation
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UClass* AttackModeAnimationBP;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UClass* NormalModeAnimationBP;

	bool bAttackMode = false;
};
