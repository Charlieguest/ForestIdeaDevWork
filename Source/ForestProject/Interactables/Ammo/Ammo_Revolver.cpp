// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo_Revolver.h"


// Sets default values
AAmmo_Revolver::AAmmo_Revolver()
{
	
}

void AAmmo_Revolver::ApplyProjectileDamage_Implementation(float damage, AActor* HitActor)
{
	Super::ApplyProjectileDamage_Implementation(damage, HitActor);
}

// Called when the game starts or when spawned
void AAmmo_Revolver::BeginPlay()
{
	Super::BeginPlay();
}
