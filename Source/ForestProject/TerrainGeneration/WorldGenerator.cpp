// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"


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

void AWorldGenerator::GenerateTerrain(int inSectionX, int inSectionY)
{
	
}

