// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerator.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"


// Sets default values
AWorldGenerator::AWorldGenerator()
{
	_BPProcTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
	_BPProcTerrain->SetupAttachment(GetRootComponent());

	_CodeProcTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SeperateTerrainMesh"));
	//_CodeProcTerrain->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWorldGenerator::GenerateTerrain(int SectionIndexX, int SectionIndexY)
{
	// Calculating the offset based on which tile section we are on
	FVector tileOffset = FVector(SectionIndexX * (XVertexCount - 1), SectionIndexY * (YVertexCount - 1), 0) * CellSize;

	//Verticies variables, the array and the single vertex variable to
	// handle the calculations to then add to the array
	TArray<FVector> verticies;
	FVector vertex;

	TArray<FVector2D> uVs;
	FVector2D uV;

	TArray<int32> triangles;

	//These play the role of "big normals" and "big tangents"
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	
	//2D For loop to calculate Verticies and UVs
	//Starting from -1 as we want to include every vertex
	//Looping from the 0th vertex position
	for(int32 IVY = -1; IVY <= YVertexCount; IVY++)
	{
		for(int32 IVX = -1; IVX <= XVertexCount; IVX++)
		{
			//calculating Vertex
			vertex = FVector(
				IVX * CellSize + tileOffset.X,
				IVY * CellSize + tileOffset.Y,
				GetHeight(FVector2d(vertex.X, vertex.Y)));
			verticies.Add(vertex);

			//Calculating UV
			uV.X = (IVX + SectionIndexX * (XVertexCount - 1)) * CellSize / 100;
			uV.Y = (IVY + SectionIndexY * (YVertexCount - 1)) * CellSize / 100;
			uVs.Add(uV);
		}
	}

	//2D for loop to calculate all of the traingles
	for(int32 ITY = 0; ITY <= YVertexCount; ITY++)
	{
		for(int32 ITX = 0; ITX <= XVertexCount; ITX++)
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

	//Subset variables of the smaller piece of tile
	TArray<FVector> subVerticies;
	TArray<FVector2D> subUVs;
	TArray<int32> subTriangles;
	TArray<FVector> subNormals;
	TArray<FProcMeshTangent> subTangents;

	int vertexIndex = 0;
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uVs, normals, tangents);

	//Filtering subset verticies and uvs
	for(int32 IVY = -1; IVY <= YVertexCount; IVY++ )
	{
		for(int32 IVX = -1; IVX <= XVertexCount; IVX++)
		{
			if(-1 < IVY && IVY < YVertexCount && -1 < IVX && IVX < XVertexCount)
			{
				subVerticies.Add(verticies[vertexIndex]);
				subUVs.Add(uVs[vertexIndex]);
				subNormals.Add(normals[vertexIndex]);
				subTangents.Add(tangents[vertexIndex]);
			}
			//Increment index every loop to avoid adding the same value twice
			vertexIndex++;
		}
	}

	//2D for loop to calculate the subset of traingles
	for(int32 ITY = 0; ITY <= YVertexCount - 2; ITY++)
	{
		for(int32 ITX = 0; ITX <= XVertexCount - 2; ITX++)
		{
			//Calulating triangle points from the bottom half of the square
			subTriangles.Add(ITX + ITY * XVertexCount);
			subTriangles.Add(ITX + ITY * XVertexCount + XVertexCount);
			subTriangles.Add(ITX + ITY * XVertexCount + 1);

			//Calculating triangle points from the top half
			subTriangles.Add(ITX + ITY * XVertexCount + XVertexCount);
			subTriangles.Add(ITX + ITY * XVertexCount + XVertexCount + 1);
			subTriangles.Add(ITX + ITY * XVertexCount + 1);
		}
	}

	//Create mesh section
	//UProceduralMeshComponent* procTerrain =  NewObject<UProceduralMeshComponent>(this, "ProceduralMesh");
	_CodeProcTerrain.Get()->CreateMeshSection(MeshSectionIndex, subVerticies, subTriangles, subNormals, subUVs, TArray<FColor>(), subTangents, true);
	
	if(_ProcTerrianMat)
	{
		_CodeProcTerrain->SetMaterial(MeshSectionIndex, _ProcTerrianMat);
	}

	//Increment mesh section index
	//So me move on to the next mesh tile
	MeshSectionIndex++;
}

float AWorldGenerator::GetHeight(FVector2D Location)
{
	return
	{
		PerlinNoiseExtended(Location, 0.00001f, 20000.0f, FVector2d(.1f)) +
		PerlinNoiseExtended(Location, 0.0001f, 10000.0f, FVector2d(.25f)) +
		PerlinNoiseExtended(Location, 0.001f, 450.0f, FVector2d(.3f))
	};
}	

float AWorldGenerator::PerlinNoiseExtended(const FVector2d Location, const float Scale, const float Amplitude,
	const FVector2d Offset)
{
	return FMath::PerlinNoise2D(Location * Scale + FVector2d(0.1f, 0.1f) + Offset) * Amplitude;
}

