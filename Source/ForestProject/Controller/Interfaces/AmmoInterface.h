// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AmmoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAmmoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FORESTPROJECT_API IAmmoInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyProjectileDamage(float damage, AActor* HitActor);
};
