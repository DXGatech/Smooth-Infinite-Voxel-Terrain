// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyVox_Test.h"
#include "SimplexNoiseBPLibrary.h"
#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
#include "PolyVox/Vector.h"
#include "RawVolumeChunk.h"



void ARawVolumeChunk::createHeightmapInVolume(PolyVox::RawVolume<uint8_t> &volData, float fRadius)
{
    for (int z = 0; z < static_cast<uint8>(fRadius); z++)
    {
        for (int y = 0; y < static_cast<uint8>(fRadius); y++)
        {
            for (int x = 0; x < static_cast<uint8>(fRadius); x++)
            {
                float tnoise = 0.0f;
                if(true){
                    tnoise = USimplexNoiseBPLibrary::SimplexNoise3D(x * 0.055, y * 0.055, z * 0.065) * 15.0f * 0.6f;
                    tnoise = noisecalc->calc2d(actx, acty, 0.0005, amp * 2.0f);
                    delete noisecalc;
                }
                auto EvaluatedNoise = 100000000.0f;
                auto EvaluatedNoise = tnoise;
                if(true){
                    EvaluatedNoise += USimplexNoiseBPLibrary::SimplexNoise2D(x * 0.05, y * 0.05/*, actz * freq*/) * 15.0f * 2.0f;
                }
                bool bSolid = z < (EvaluatedNoise * 0.95) * 0.65f;
                bool bFoliage = (EvaluatedNoise - z) < 10.0f;
                volData.setVoxel(x, y, z, (bSolid ? 255 : 0));
            }
        }
    }
}
void ARawVolumeChunk::createSphereInVolume(PolyVox::RawVolume<uint8_t> &volData, float fRadius)
{
    FVector worldLoc = GetActorLocation();
    PolyVox::Vector3DFloat v3dVolCenter(volData.getWidth() / 2 + worldLoc.X, volData.getHeight() / 2 + worldLoc.Y, volData.getDepth() / 2 + worldLoc.Z);
    for (int z = 0; z < volData.getDepth(); z++)
    {
        for (int y = 0; y < volData.getHeight(); y++)
        {
            for (int x = 0; x < volData.getWidth(); x++)
            {
                PolyVox::Vector3DFloat v3dCurrentPos(x, y, z);
                float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();
                uint8_t uVoxelValue = 255;
                if (fDistToCenter <= fRadius)
                {
                  uVoxelValue = 255;
                }
                volData.setVoxel(x, y, z, uVoxelValue);
            }
        }
    }
}


void ARawVolumeChunk::createBlockInVolume(PolyVox::RawVolume<uint8_t>& volData, float fRadius)
{
     FVector worldLoc = GetActorLocation();
    PolyVox::Vector3DFloat v3dVolCenter(volData.getWidth() / 2 + worldLoc.X, volData.getHeight() / 2 + worldLoc.Y, volData.getDepth() / 2 + worldLoc.Z);
    for (int z = 0; z < static_cast<uint8>(fRadius); z++)
    {
        for (int y = 0; y < static_cast<uint8>(fRadius); y++)
        {
            for (int x = 0; x < static_cast<uint8>(fRadius); x++)
            {
                PolyVox::Vector3DFloat v3dCurrentPos(x, y, z);
                float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();
                uint8_t uVoxelValue = 255;
                volData.setVoxel(x, y, z, uVoxelValue);
            }
        }
    }
}
ARawVolumeChunk::ARawVolumeChunk()
{
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("chunk sphere"));
}


void ARawVolumeChunk::ModifyChunk(FVector WorldLoc)
{
    PolyVox::Region RegionToEdit(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(20, 20, 20)));
    PolyVox::RawVolume<uint8_t> volDataEdit(RegionToEdit);
    for(int x = 10; x < 20; x++){
        for(int y = 10; y < 20; y++){
            for(int z = 10; z < 20; z++){
                uint8_t uVoxelValue = 255;
                volData.Get()->setVoxel(x, y, z, uVoxelValue);
                
            }
        }
    }
    FVector chunkLocation = GetActorLocation();
    FVector Loc = FTransform(chunkLocation).InverseTransformPosition(WorldLoc);
    FVector Loc = WorldLoc / 50.0f;
    float x = std::abs(Loc.X);
    float y = std::abs(Loc.Y);
    float z = std::abs(Loc.Z);
    Loc.X = x;
    Loc.Y = y;
    Loc.Z = z;
    UE_LOG(LogTemp, Error, TEXT("Location: %s"), *(Loc.ToString()))
    UE_LOG(LogTemp, Error, TEXT("X: %x"), x)
    volData.Get()->setVoxel(x, y, z, 0);
    auto mesh = extractCubicMesh(volData.Get(), volData.Get()->getEnclosingRegion());
    auto decodedMesh = decodeMesh(mesh);
    int Index = decodedMesh.getNoOfIndices();
    UE_LOG(LogTemp, Error, TEXT("triangle num : %i"), Index)
    auto Vertices = TArray<FVector>();
    auto Indices = TArray<int32>();
    auto Normals = TArray<FVector>();
    auto UV0 = TArray<FVector2D>();
    auto Colors = TArray<FColor>();
    auto Tangents = TArray<FProcMeshTangent>();
    if(Index >= 2){
        for (uint32 i = 0; i < Index - 2; i+=3)
        {
            auto Index = decodedMesh.getIndex(i + 2);
            auto Vertex2 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
            Index = decodedMesh.getIndex(i + 1);
            auto Vertex1 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
            Index = decodedMesh.getIndex(i);
            auto Vertex0 = decodedMesh.getVertex(Index);
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
    Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
}
void ARawVolumeChunk::BeginPlay()
{
	Super::BeginPlay();
    
    FVector Location = GetActorLocation();
    float x = Location.X;
    float y = Location.Y;
    float z = Location.Z;
    TSharedPtr<PolyVox::RawVolume<uint8_t>>RawVolTest (new PolyVox::RawVolume<uint8_t>(PolyVox::Region(PolyVox::Vector3DInt32(-50, -50, -50), PolyVox::Vector3DInt32(200, 200, 200))));
    volData = RawVolTest;
    PolyVox::RawVolume<uint8_t> volData(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63)));
    volData = new PolyVox::RawVolume<uint8_t>(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63)));
    VolumePtr = &volData;
    PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(63, 63, 63))
    createHeightmapInVolume(RawVolTest.ToSharedRef().Get(), 200);
    auto mesh = extractMarchingCubesMesh(RawVolTest.Get(), RawVolTest.Get()->getEnclosingRegion());
    auto decodedMesh = decodeMesh(mesh);
    int Index = decodedMesh.getNoOfIndices();
    auto Vertices = TArray<FVector>();
    auto Indices = TArray<int32>();
    auto Normals = TArray<FVector>();
    auto UV0 = TArray<FVector2D>();
    auto Colors = TArray<FColor>();
    auto Tangents = TArray<FProcMeshTangent>();
    if(Index >= 2){
        for (uint32 i = 0; i < Index - 2; i+=3)
        {
            auto Index = decodedMesh.getIndex(i + 2);
            auto Vertex2 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex2.position) * 100.f));
            Index = decodedMesh.getIndex(i + 1);
            auto Vertex1 = decodedMesh.getVertex(Index);
            Indices.Add(Vertices.Add(FPolyVoxVector(Vertex1.position) * 100.f));
            Index = decodedMesh.getIndex(i);
            auto Vertex0 = decodedMesh.getVertex(Index);
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
    Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
}

void ARawVolumeChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

