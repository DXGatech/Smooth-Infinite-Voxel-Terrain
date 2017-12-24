// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyVox_Test.h"
#include "SimplexNoiseBPLibrary.h"
//#include "Engine.h"
#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
#include "NoiseCalculator.h"
#include "PolyVox/Vector.h"
#include "PolyVox/BaseVolume.h"
#include "Header.h"
#include "TerrainGenManager.h"
ATerrainGenManager::ATerrainGenManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain generation"));
    Foliage = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Foliage"));

}

void ATerrainGenManager::EditVoxelAtLoc(FVector Loc)
{
    float locx = Loc.X;
    float locy = Loc.Y;
    float locz = Loc.Z;
    
    PolyVox::MaterialDensityPair44 Air;
    Air.setMaterial(0);
    Air.setDensity(0);
    UE_LOG(LogTemp, Error, TEXT("chunk fullfilled"))
    
    FVector UpdateLoc = GetActorLocation();
    //float upx = UpdateLoc.X / 200.0f;
    //float upy = UpdateLoc.Y / 200.0f;
    float upx = genlocx;
    float upy = genlocx;
    float upz = -50.0f;
    
    PolyVox::Region ToExtract(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(sizevec_x, sizevec_y, sizevec_z + upz));
    PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>* VoxChunk = VoxelVolume.Get();
    //ToExtract.getLowerX() - 5
    
    for (int x = 0; x <= 30; x++)
    {
        for (int y = 0; y <= 30; y++)
        {
            for (int z = 0; z <= 30; z++) {
                VoxChunk->setVoxel(x, y, z, Air);
            }
        }
    }

    auto Vertices = TArray<FVector>();
    auto Indices = TArray<int32>();
    auto Normals = TArray<FVector>();
    auto UV0 = TArray<FVector2D>();
    auto Colors = TArray<FColor>();
    auto Tangents = TArray<FProcMeshTangent>();
    
    auto ExtractedMesh = PolyVox::extractCubicMesh(VoxelVolume.Get(), ToExtract);
    //auto ExtractedMesh = PolyVox::extractMarchingCubesMesh(VoxelVolume.Get(), ToExtract);
    auto DecodedMesh = decodeMesh(ExtractedMesh);
    auto Index = DecodedMesh.getNoOfIndices();
    UE_LOG(LogTemp, Error, TEXT("number of indices after edit: %d"), Index)
    
    if(Index >= 2){
        for (uint32 i = 0; i < Index - 2; i+=3)
        {
            auto Index = DecodedMesh.getIndex(i + 2);
            auto Vertex2 = DecodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
            
            Index = DecodedMesh.getIndex(i + 1);
            auto Vertex1 = DecodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
            
            Index = DecodedMesh.getIndex(i);
            auto Vertex0 = DecodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex0.position) * 100.f));
            
            const FVector Edge01 = FPolyVoxVector(Vertex1.position - Vertex0.position);
            const FVector Edge02 = FPolyVoxVector(Vertex2.position - Vertex0.position);
            const FVector TangentX = Edge01.GetSafeNormal();
            FVector TangentZ = (Edge01 ^ Edge02).GetSafeNormal();
            
            for (int32 j = 0; j < 3; j++)
            {
                Tangents.Add(FProcMeshTangent(TangentX, false));
                Normals.Add(TangentZ);
            }
        }
    }
    Mesh->ClearMeshSection(0);
    Mesh->CreateMeshSection(0, Vertices, Indices,  Normals, UV0, Colors, Tangents, true);
}

// Called when the game starts or when spawned
void ATerrainGenManager::BeginPlay()
{
    Super::BeginPlay();
    
	
}

// Called every frame
void ATerrainGenManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

VoxelTerrainPager::VoxelTerrainPager(int NoiseSeed, int Octaves, float Frequency, float Scale, float Offset, float Height, float x, float y, float z, float inamp, float infreq, float inwaterlevel) : PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Pager(), Seed(NoiseSeed), NoiseOctaves(Octaves), NoiseFrequency(Frequency), NoiseScale(Scale), NoiseOffset(Offset), TerrainHeight(Height)
{
    posx = x;
    posy = y;
    posz = z;
    amp = inamp;
    freq = infreq;
    waterlevel = inwaterlevel;
}
void ATerrainGenManager::SetGenLoc(float x, float y, float z)
{
    genlocx = x;
    genlocy = y;
    PolyVox::Region ToExtract(PolyVox::Vector3DInt32(x, y, z), PolyVox::Vector3DInt32(x + sizevec_x, y + sizevec_y, z + sizevec_z));
    TSharedPtr<PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>>PolyVoxelVolume = MakeShareable(new PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>(new VoxelTerrainPager(0, 0, 0, 0, 0, 0, x, y, z, amplitude, NoiseFrequency, waterlevel)));
    VoxelVolume = PolyVoxelVolume;
    auto ExtractedMesh = PolyVox::extractMarchingCubesMesh(PolyVoxelVolume.Get(), ToExtract);
    auto DecodedMesh = decodeMesh(ExtractedMesh);
    for(int Material = 0; Material < TerrainMaterial.Num(); Material++){
    
        auto Vertices = TArray<FVector>();
        auto Indices = TArray<int32>();
        auto Normals = TArray<FVector>();
        auto UV0 = TArray<FVector2D>();
        auto Colors = TArray<FColor>();
        auto Tangents = TArray<FProcMeshTangent>();
    

        auto Indexmesh = DecodedMesh.getNoOfIndices();
        UE_LOG(LogTemp, Error, TEXT("index num: %i"), Indexmesh)
        if(Indexmesh >= 2){
            for (uint32 i = 0; i < Indexmesh - 2; i+=3)
            {
                // We need to add the vertices of each triangle in reverse or the mesh will be upside down
                auto Index = DecodedMesh.getIndex(i + 2);
                auto Vertex2 = DecodedMesh.getVertex(Index);
                auto TriangleMaterial = Vertex2.data.getMaterial();
                if (TriangleMaterial == (Material + 1))
                {
                    Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
                    Index = DecodedMesh.getIndex(i + 1);
                    auto Vertex1 = DecodedMesh.getVertex(Index);
                    Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
                    Index = DecodedMesh.getIndex(i);
                    auto Vertex0 = DecodedMesh.getVertex(Index);
                    Indices.Add(Vertices.Add(FPolyVoxVector(Vertex0.position) * 100.f));
                    const FVector Edge01 = FPolyVoxVector(Vertex1.position - Vertex0.position);
                    const FVector Edge02 = FPolyVoxVector(Vertex2.position - Vertex0.position);
                    const FVector TangentX = Edge01.GetSafeNormal();
                    FVector TangentZ = (Edge01 ^ Edge02).GetSafeNormal();
                    for (int32 i = 0; i < 3; i++)
                    {
                        Tangents.Add(FProcMeshTangent(TangentX, false));
                        Normals.Add(TangentZ);
                    }
                }
            }
            Mesh->CreateMeshSection(Material, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
            Mesh->SetMaterial(Material, TerrainMaterial[Material]);
        }
        
        Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
        Mesh->SetMaterial(0, TerrainMaterial[Material]);
    }
    FVector fStart = GetActorLocation() + FVector(FMath::RandRange(100.0f, 500.0f), FMath::RandRange(100.0f, 500.0f), 50000.0f);
    FVector fHit = fStart - FVector(0.0f, 0.0f, 80000.0f);
    
    FHitResult HitResult;
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetWorld()->GetFirstPlayerController()->GetCharacter());
    GetWorld()->LineTraceSingleByObjectType(
                                            HitResult,
                                            fStart,
                                            fHit,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_MAX),
                                            TraceParameters
                                            );
    FVector FoliageLoc = HitResult.Location;
    FTransform FoliageTransform = FTransform(FoliageLoc);
    if(FMath::RandRange(10, 50) > 30 && FoliageLoc.Z > 2000.0f)
        Foliage->AddInstanceWorldSpace(FoliageTransform);
    //UE_LOG(LogTemp, Error, TEXT("Foliage hit loc: %s"), *(FoliageTransform.ToString()))
}

void ATerrainGenManager::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void VoxelTerrainPager::pageIn(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Chunk* Chunk)
{
    region.getUpper_();
    NoiseCalculator* noisecalc = new NoiseCalculator();
    for (int x = region.getLowerX(); x <= region.getUpperX(); x++)
    {
        for (int y = region.getLowerY(); y <= region.getUpperY(); y++)
        {
            for (int z = region.getLowerZ(); z <= region.getUpperZ(); z++) {
                float actx = x + posx;
                float acty = y + posy;
                float actz = z + posz;
                float tnoise = 0.0f;
                if(z > 0){//z > 30
                    tnoise = USimplexNoiseBPLibrary::SimplexNoise3D(actx * 0.055, acty * 0.055, actz * 0.065) * amp * 0.6f;
                }
                auto EvaluatedNoise = tnoise;
                if(z > 0){
                    EvaluatedNoise += USimplexNoiseBPLibrary::SimplexNoise2D(actx * freq, acty * freq/*, actz * freq*/) * amp * 2.0f;
                }
                
                bool bSolid = actz < (EvaluatedNoise * 0.95) * 0.65f;
                bool bFoliage = (EvaluatedNoise - actz) < 10.0f;
                PolyVox::MaterialDensityPair44 Voxel;
                Voxel.setDensity(bSolid ? 255 : 0);
                if(bSolid){
                    
                    if(z <= 3)
                    {
                        Voxel.setMaterial(3);//3
                        Voxel.setDensity(255);
                    }else{
                        Voxel.setMaterial(2);//2
                    }
                }else{
                    Voxel.setMaterial(0);
                }
                Chunk->setVoxel(x - region.getLowerX(), y - region.getLowerY(), z - region.getLowerZ(), Voxel);
            }
        }
    }
}
void VoxelTerrainPager::pageOut(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair44>::Chunk* Chunk)
{
    
    
}
