// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"


#include "WorldGenerator.generated.h"

UCLASS()
class FORESTPROJECT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int YVertexCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int XVertexCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellSize = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumOfSectionsX = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumOfSectionsY = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MeshSectionIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProceduralMeshComponent> _ProcTerrain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> _ProcTerrianMat = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GenerateTerrain(int SectionIndexX, int SectionIndexY);

	UFUNCTION(BlueprintCallable)
	float GetHeight(FVector2D UVs);
};
