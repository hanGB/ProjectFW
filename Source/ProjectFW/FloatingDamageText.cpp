// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingDamageText.h"
#include "Components/WidgetComponent.h"

// Sets default values
AFloatingDamageText::AFloatingDamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageWidget = CreateDefaultSubobject<UWidgetComponent>("Damage Widget");
	DamageWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFloatingDamageText::BeginPlay()
{
	Super::BeginPlay();
	
	Timer = 0.0f;
}

// Called every frame
void AFloatingDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Timer += DeltaTime;

	if (Timer > LifeTime)
	{
		Destroy();
	}
}

