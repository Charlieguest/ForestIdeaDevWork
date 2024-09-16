// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShootingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UShootingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FORESTPROJECT_API IShootingInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireWeapon(AActor* Weapon);
};