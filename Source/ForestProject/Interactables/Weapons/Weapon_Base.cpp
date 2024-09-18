﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Base.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	AActor* _PlayerCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	_PlayerCharacter = Cast<AThePlayerCharacter>(_PlayerCharacterRef);

	//This will receive the ItemUse Delegate
	_PlayerCharacter->OnItemUse.AddDynamic(this, &AWeapon_Base::UseItem);
	
	//Actors to ignore with raycasts
	_ActorsToIgnore.Add(GetOwner());
	_ActorsToIgnore.Add(_PlayerCharacter);
	_ActorsToIgnore.Add(this);
}

void AWeapon_Base::UseItem_Implementation(AActor* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Firing!"));
	if(Item == this)
	{
	}
}
