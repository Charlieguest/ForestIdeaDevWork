// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ammo_Base.h"
#include "Ammo_Revolver.generated.h"

UCLASS()
class FORESTPROJECT_API AAmmo_Revolver : public AAmmo_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmo_Revolver();

	virtual void ApplyProjectileDamage_Implementation(float damage, AActor* HitActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};