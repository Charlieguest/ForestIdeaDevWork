// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForestProject/Controller/Interfaces/ItemUseInterface.h"
#include "ForestProject/PlayerCharacter/ThePlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS(Abstract)
class FORESTPROJECT_API AWeapon_Base : public AActor, public IItemUseInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _StaticMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> _Collider;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> _Arrow;
	
	// Sets default values for this actor's properties
	AWeapon_Base();
	
	virtual void UseItem_Implementation(AActor* Item) override;

	UPROPERTY()
	TObjectPtr<AThePlayerCharacter> _PlayerCharacter;

	UPROPERTY()
	FVector _CameraLocation;
	UPROPERTY()
	FRotator _CameraRotation;
	UPROPERTY()
	FVector _CameraDistance;
	
	UPROPERTY()
	float _ItemRange = 2000.0f;
	UPROPERTY()
	TArray<AActor*> _ActorsToIgnore;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};