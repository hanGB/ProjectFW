// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	Ammo = MaxAmmo;
	bReloading = false;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bReloading)
	{
		Reload(DeltaTime);
	}
}

void AGun::Draw(bool bDraw)
{
	Mesh->SetVisibility(bDraw);
}

bool AGun::PullTrigger()
{
	if (bReloading)
	{
		return false;
	}

	if (MuzzleEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleEffect, Mesh, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	}

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AController* OwnerController = OwnerPawn->GetController())
		{
			FVector Location;
			FRotator Rotation;
			OwnerController->GetPlayerViewPoint(Location, Rotation);

			FVector End = Location + Rotation.Vector() * MaxRange;

			FHitResult HitResult;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1))
			{
				if (ImpactEffect)
				{
					FVector ShootDirection = -Rotation.Vector();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, HitResult.Location, ShootDirection.Rotation());

					if (AActor* HitActor = HitResult.GetActor())
					{
						FPointDamageEvent DamageEvent(Damage, HitResult, ShootDirection, nullptr);
						HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
					}
				}
			}
		}
	}

	Ammo--;
	if (Ammo == 0)
	{
		ReloadState = 0.0f;
		bReloading = true;
	}

	return true;
}

bool AGun::IsDrawed() const
{
	// 메쉬가 보일 경우 꺼내진 상태
	return Mesh->GetVisibleFlag();
}

float AGun::GetAmmoRate() const
{
	if (bReloading)
	{
		return ReloadState;
	}
	else
	{
		return (float)Ammo / (float)MaxAmmo;
	}
}

void AGun::AddAmmoWhenRest(float DeltaTime)
{
	RestTimer += DeltaTime;
	if (RestAmmoTime > RestTimer) return;

	RestTimer = 0.0f;
	if (Ammo < MaxAmmo)
	{
		Ammo++;
	}
}

void AGun::ResetRestTimer()
{
	RestTimer = 0.0f;
}

bool AGun::IsReloading() const
{
	return bReloading;
}

void AGun::Reload(float DeltaTime)
{
	ReloadState += DeltaTime * (1.0f / ReloadTime);

	if (ReloadState >= 1.0f)
	{
		Ammo = MaxAmmo;

		ReloadState = 0.0f;
		bReloading = false;
	}
}

