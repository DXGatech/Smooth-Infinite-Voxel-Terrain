// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NoiseCalculator.h"
#include "TestActor.generated.h"

UCLASS()
class POLYVOX_TEST_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();
    
    UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
    ATerrainGenManager* multi_chunkgen(FVector loc);
    
    UPROPERTY(Category = "Voxel Terrain", EditAnywhere)
    float sleep_seconds_first = 0.065f;
    
    UPROPERTY(Category = "Voxel Terrain", EditAnywhere)
    float sleep_seconds_sec = 0.065f;
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) TArray<UMaterialInterface*> TerrainMaterial;
    

	
	
};
