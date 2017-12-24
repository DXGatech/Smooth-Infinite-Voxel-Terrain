// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PolyVox/PagedVolume.h"
#include "Engine.h"
//#include "Header.h"
#include "ProceduralMeshComponent.h"
#include "PolyVox/MaterialDensityPair.h"
#include "TerrainGenManager.generated.h"

class VoxelTerrainPager : public PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Pager
{
public:
    // Constructor
    VoxelTerrainPager(int NoiseSeed = 123, int Octaves = 3, float Frequency = 0.01, float Scale = 32, float Offset = 0, float Height = 64, float x = 0, float y = 0, float z = 0, float inamp = 25, float infreq = 0.05, float waterlevel = 500.0f);
    
    VoxelTerrainPager(float x, float y, float z);
    
    // Destructor
    virtual ~VoxelTerrainPager() {};
    
    void DeleteVoxelAtLoc(float x, float y, float z);
    
    // PagedVolume::Pager functions
    virtual void pageIn(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Chunk* pChunk);
    virtual void pageOut(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Chunk* pChunk);
    
private:
    
    
    PolyVox::MaterialDensityPair44 DeleteVoxel;
    

    uint32 Seed = 123;
    uint32 NoiseOctaves = 3;
    float NoiseFrequency = 0.05;
    float NoiseScale = 32;
    float NoiseOffset = 0;
    float TerrainHeight = 64;
    
    float waterlevel = 500.0f;
    
    float freq;
    float amp;
    
    float posx = 0;
    float posy = 0;
    float posz = 0;
};


UCLASS()
class POLYVOX_TEST_API ATerrainGenManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainGenManager();
    
    float sleeptime = 0.1f;
    
    
    // Called after the C++ constructor and after the properties have been initialized.
    virtual void PostInitializeComponents() override;
    
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float sizevec_x = 10;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float sizevec_y = 10;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float sizevec_z = 100;
    
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float waterlevel = 500.0f;
    
    UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
    void SetGenLoc(float x, float y, float z);
    
    UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
    void updateMesh();
    
    UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
    void initialize(float x, float y, float z);
    
    UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
    void initialize_multi(float x, float y, float z);
    
    TArray<FVector> Vertices = TArray<FVector>();
    TArray<int32> Indices = TArray<int32>();
    TArray<FVector> Normals = TArray<FVector>();
    TArray<FVector2D> UV0 = TArray<FVector2D>();
    TArray<FColor> Colors = TArray<FColor>();
    TArray<FProcMeshTangent> Tangents = TArray<FProcMeshTangent>();
    
    float spawnx, spawny, spawnz;
    
    PolyVox::Region iniRegion;
    
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) TArray<UMaterialInterface*> TerrainMaterial;
    
    //UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
    //void Multi_setGenLoc(float x, float y, float z);
    
    
    UFUNCTION(BlueprintCallable)
    void EditVoxelAtLoc(FVector Loc);
    
    float genlocx;
    float genlocy;
    
    
    //UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) bool bMarchingCubes = false;
    
    //PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>OrgVoxVol;
    
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float threshold;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float frequency_x;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float frequency_y;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float frequency_z;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float amplitude = 10;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) int32 Seed;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) int32 NoiseOctaves;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float NoiseFrequency = 0.01;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float NoiseScale;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float NoiseOffset;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float TerrainHeight;
    
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, VisibleAnywhere)
    UProceduralMeshComponent* Mesh = nullptr;
    UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, VisibleAnywhere)
    UInstancedStaticMeshComponent* Foliage = nullptr;

    TSharedPtr<PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>> VoxelVolume;
    
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
