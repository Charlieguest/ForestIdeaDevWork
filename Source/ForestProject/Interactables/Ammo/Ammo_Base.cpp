// Fill out your copyright notice in the Description page of Project Settings.

#include "Ammo_Base.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AAmmo_Base::AAmmo_Base()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);
	
	_Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collider->SetupAttachment(_Root);

	_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

void AAmmo_Base::ApplyProjectileDamage_Implementation(float damage, AActor* HitActor)
{
	IAmmoInterface::ApplyProjectileDamage_Implementation(damage, HitActor);
}

// Called when the game starts or when spawned
void AAmmo_Base::BeginPlay()
{
	Super::BeginPlay();
}