// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon_Base.h"
#include "Weapon_Revolver.generated.h"

UCLASS()
class FORESTPROJECT_API AWeapon_Revolver : public AWeapon_Base
{
	GENERATED_BODY()

public:

	virtual void FireWeapon_Implementation(AActor* Item) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
