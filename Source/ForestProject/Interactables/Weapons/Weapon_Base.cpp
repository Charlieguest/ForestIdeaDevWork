// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Base.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	_Collider -> SetupAttachment(_Root);

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_StaticMesh -> SetupAttachment(_Root);

	_Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	_Arrow->SetupAttachment(_Root);
}

void AWeapon_Base::FireWeapon_Implementation(AActor* Weapon)
{
	IShootingInterface::FireWeapon_Implementation(Weapon);
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}
