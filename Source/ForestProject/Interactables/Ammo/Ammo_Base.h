// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForestProject/Controller/Interfaces/AmmoInterface.h"
#include "GameFramework/Actor.h"
#include "Ammo_Base.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class FORESTPROJECT_API AAmmo_Base : public AActor, public IAmmoInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> _Collider;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> _ProjectileMovement;

public:
	// Sets default values for this actor's properties
	AAmmo_Base();

	virtual void ApplyProjectileDamage_Implementation(float damage, AActor* HitActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};