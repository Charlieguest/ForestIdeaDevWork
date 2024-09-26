// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerator.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Components/SplineComponent.h"


// Sets default values
AWorldGenerator::AWorldGenerator()
{
	_BPProcTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
	_BPProcTerrain->SetupAttachment(GetRootComponent());

	_CodeProcTerrain = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SeperateTerrainMesh"));
	//_CodeProcTerrain->SetupAttachment(GetRootComponent());

	_PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	_PathSpline->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	_PathStartPoints.Empty();
	_PathEndPoints.Empty();
	
	if(_RandomiseTerrainLayout)
	{
		_PerlinNoiseOffset = FVector2d(FMath::RandRange(0.f, 10000.f),FMath::RandRange(0.f, 10000.f));
		//Height
		_MountainHeight = _MountainHeight * FMath::RandRange(0.5f, 0.8f);
		_BoulderHeight = _BoulderHeight * FMath::RandRange(0.5f, 1.0f);
		//Scale
		_MountainScale = _MountainScale * FMath::RandRange(0.5f, 0.8f);
		_BoulderScale = _BoulderScale * FMath::RandRange(0.6f, 1.0f);
		_RockScale = _RockScale * FMath::RandRange(0.8f, 1.3f);
	}
	
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
		    vertex.X = IVX * CellSize + tileOffset.X,
			vertex.Y = IVY * CellSize + tileOffset.Y,
			vertex.Z = GetHeight(FVector2d(vertex.X, vertex.Y));

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
	for(int32 IVY = -1; IVY <= YVertexCount; IVY++)
	{
		for(int32 IVX = -1; IVX <= XVertexCount; IVX++)
		{
			if(-1 < IVY && IVY < YVertexCount && -1 < IVX && IVX < XVertexCount)
			{
				subVerticies.Add(verticies[vertexIndex]);
				subUVs.Add(uVs[vertexIndex]);
				subNormals.Add(normals[vertexIndex]);
				subTangents.Add(tangents[vertexIndex]);

				//Calls function that calculates available path start/end points
				CalculatePathPoints(verticies[vertexIndex], SectionIndexX, SectionIndexY, IVX, IVY);
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
		PerlinNoiseExtended(Location, 1.f/_MountainScale, _MountainHeight, FVector2d(.05f)) +
		PerlinNoiseExtended(Location, 1.f/_BoulderScale, _BoulderHeight, FVector2d(.16f)) +
		PerlinNoiseExtended(Location, 1.f/_RockScale, 350.0f, FVector2d(.3f))
	};
}	

float AWorldGenerator::PerlinNoiseExtended(const FVector2d Location, const float Scale, const float Amplitude,
	const FVector2d Offset)
{
	return FMath::PerlinNoise2D(Location * Scale + FVector2d(0.1f, 0.1f) + Offset + _PerlinNoiseOffset) * Amplitude;
}

/// Calculates the perimeter points for the trail start position
/// Everything else "Within reason" is added to the end position
/// 
/// Since the terrain itself is generated in "tiles" we need to
/// check which tile we are in before we choose which edges to select
/// (if it's not a middle tile and we have no edges to select at all)
void AWorldGenerator::CalculatePathPoints(const FVector TerrainVertex, const int XsectionIndex, const int YSectionIndex, const int CurrentVertexCountX, const int CurrentVertexCountY)
{
	//Is it a top left corner tile
	if(XsectionIndex == 0 && YSectionIndex == 0)
	{
		if(CurrentVertexCountX == 0 || CurrentVertexCountY == 0)
		{
			_PathStartPoints.Add(TerrainVertex);
		}
		//Making sure endpoint isn't too close to the edge
		else if(
			CurrentVertexCountX > 9 &&
			CurrentVertexCountX < XVertexCount - 10 &&
			CurrentVertexCountY > 9 &&
			CurrentVertexCountY < YVertexCount - 10
			)
		{
			_PathEndPoints.Add(TerrainVertex);
		}
	}
	//Is it a bottom right corner tile
	else if(XsectionIndex == NumOfSectionsX - 1 && YSectionIndex == NumOfSectionsY - 1)
	{
		if(CurrentVertexCountX == XVertexCount - 1 || CurrentVertexCountY == YVertexCount - 1)
		{
			_PathStartPoints.Add(TerrainVertex);
		}
		//Making sure endpoint isn't too close to the edge
		else if(
			CurrentVertexCountX > 9 &&
			CurrentVertexCountX < XVertexCount - 10 &&
			CurrentVertexCountY > 9 &&
			CurrentVertexCountY < YVertexCount - 10
			)
		{
			_PathEndPoints.Add(TerrainVertex);
		}
	}
	//Is it a top right corner tile
	else if(XsectionIndex == NumOfSectionsX - 1 && YSectionIndex == 0)
	{
		if(CurrentVertexCountX == XVertexCount - 1 || CurrentVertexCountY == 0)
		{
			_PathStartPoints.Add(TerrainVertex);
		}
		//Making sure endpoint isn't too close to the edge
		else if(
			CurrentVertexCountX > 9 &&
			CurrentVertexCountX < XVertexCount - 10 &&
			CurrentVertexCountY > 9 &&
			CurrentVertexCountY < YVertexCount - 10
			)
		{
			_PathEndPoints.Add(TerrainVertex);
		}
	}
	//Is it a bottom left corner tile
	else if(XsectionIndex == 0 && YSectionIndex == NumOfSectionsX - 1)
	{
		if(CurrentVertexCountX == 0 || CurrentVertexCountY == YVertexCount - 1)
		{
			_PathStartPoints.Add(TerrainVertex);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Bottom Left"));
		}
		//Making sure endpoint isn't too close to the edge
		else if(
			CurrentVertexCountX > 9 &&
			CurrentVertexCountX < XVertexCount - 10 &&
			CurrentVertexCountY > 9 &&
			CurrentVertexCountY < YVertexCount - 10
			)
		{
			_PathEndPoints.Add(TerrainVertex);
		}
	}
}

void AWorldGenerator::SetPathPoints()
{
	//Setting locations of start and end point of tiles.
	FVector pathStartPoint = _PathStartPoints[FMath::RandRange(0, _PathStartPoints.Num() - 1)];
	FVector pathEndPoint;

	//Repeating until a path far enough away from the start point is found
	do
	{
		pathEndPoint = _PathEndPoints[FMath::RandRange(0, _PathEndPoints.Num() - 1)];
	} while (FMath::Abs(pathEndPoint.X - pathStartPoint.X) <= 15000.f || FMath::Abs(pathEndPoint.Y - pathStartPoint.Y) <= 15000.f);
	
	_PathSpline->SetLocationAtSplinePoint(0, pathStartPoint, ESplineCoordinateSpace::World, true);
	_PathSpline->SetLocationAtSplinePoint(1, pathEndPoint, ESplineCoordinateSpace::World, true);
}
