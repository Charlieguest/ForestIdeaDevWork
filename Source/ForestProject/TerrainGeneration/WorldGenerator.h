// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldGenerator.generated.h"

class USplineComponent;
class UProceduralMeshComponent;

UCLASS()
class FORESTPROJECT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGenerator();

	// Generator Size Vars

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
	int32 MeshSectionIndex = 0;

	//----------------------------
	//Randomisation of terrain vars
	//----------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MountainHeight = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _BoulderHeight = 7000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MountainScale = 200000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _BoulderScale = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _RockScale = 4500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D _PerlinNoiseOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _RandomiseTerrainLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProceduralMeshComponent> _BPProcTerrain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProceduralMeshComponent> _CodeProcTerrain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> _ProcTerrianMat = nullptr;
	
	//----------------------------
	//Path Generation Vars
	//----------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> _PathSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> _PathStartPoints;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> _PathEndPoints;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GenerateTerrain(const int SectionIndexX,const int SectionIndexY);

	UFUNCTION(BlueprintCallable)
	float GetHeight(FVector2D Location);

	float PerlinNoiseExtended(const FVector2d Location, const float Scale, const float Amplitude, const FVector2d Offset);

	void CalculatePathPoints(const FVector TerrainVertex, const int XsectionIndex, const int YSectionIndex, const int CurrentVertexCountX, const int CurrentVertexCountY);

	UFUNCTION(BlueprintCallable)
	void SetPathPoints();
};
