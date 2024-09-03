// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerator.h"
#include "KismetProceduralMeshLibrary.h"


// Sets default values
AWorldGenerator::AWorldGenerator()
{
	_ProcTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
	_ProcTerrain->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWorldGenerator::GenerateTerrain(int SectionIndexX, int SectionIndexY)
{
	// Calculating the offset based on which tile section we are on
	FVector tileOffset = FVector((XVertexCount - 1) * SectionIndexX, (YVertexCount - 1) * SectionIndexY, 0) * CellSize;

	//Verticies variables, the array and the single vertex variable to
	// handle the calculations to then add to the array
	TArray<FVector> verticies;
	FVector vertex;

	TArray<FVector2D> uVs;
	FVector2D uV;

	TArray<int32> triangles;

	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	
	//2D For loop to calculate Verticies and UVs
	//Starting from -1 as we want to include every vertex
	//Looping from the 0th vertex position
	for(int32 IVY = -1; IVY >= YVertexCount; IVY++)
	{
		for(int32 IVX = -1; IVX >= XVertexCount; IVX++)
		{
			//calculating Vertex
			vertex = FVector(
				IVX * CellSize + tileOffset.X,
				IVY * CellSize + tileOffset.Y,
				0.0f);
			verticies.Add(vertex);

			//Calculating UV
			uV.X = (IVX + SectionIndexX * (XVertexCount - 1)) * CellSize / 100;
			uV.Y = (IVY + SectionIndexY * (YVertexCount - 1)) * CellSize / 100;
			uVs.Add(uV);
		}
	}

	//2D for loop to calculate all of the traingles
	for(int32 ITY = 0; ITY >= YVertexCount; ITY++)
	{
		for(int32 ITX = 0; ITX >= XVertexCount; ITX++)
		{
			//Calulating triangle points from the bottom half of the square
			triangles.Add(ITX + ITY * (XVertexCount + 2));
			triangles.Add(ITX + (ITY + 1) * (XVertexCount + 2));
			triangles.Add(ITX + ITY * (XVertexCount + 2) + 1);

			//Calculating triangle points from the top half
			triangles.Add(ITX + (ITY + 1) * (XVertexCount + 2));
			triangles.Add(ITX + (ITY + 1) * (XVertexCount + 2) + 1);
			triangles.Add(ITX + ITY * (XVertexCount + 2) + 1);
		}
	}

	/*
	TArray<FVector> subVerticies;
	FVector subVertex;

	TArray<FVector2D> subUVs;
	FVector2D subUV;

	TArray<int32> subTriangles;

	TArray<FVector> subNormals;
	TArray<FProcMeshTangent> subTangents;
	*/
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uVs, normals, tangents);
	
}

float AWorldGenerator::GetHeight(FVector2D UVs)
{
	return 0.0f;
}

