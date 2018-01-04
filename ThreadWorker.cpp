// MarchingVoxels 2018. All rights reserved

#include "PolyVox_Test.h"
#include "SimplexNoiseBPLibrary.h"
#include "TestActor.h"


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ATerrainGenManager* ATestActor::multi_chunkgen(FVector loc)
{
    loc.Z = -10.0f;
    FRotator Rotation(0.0f, 0.0f, 0.0f);
    FActorSpawnParameters SpawnInfo;
    ATerrainGenManager* chunkActor = Cast<ATerrainGenManager>(GetWorld()->SpawnActor<ATerrainGenManager>(loc, Rotation, SpawnInfo));
    chunkActor->TerrainMaterial = TerrainMaterial;
    chunkActor->sizevec_x = 100;
    chunkActor->sizevec_y = 100;
    chunkActor->sizevec_z = 100;
    chunkActor->sleeptime = sleep_seconds_first;
    TQueue<NoiseCalculator, EQueueMode::Spsc>* CalcQueue = new TQueue<NoiseCalculator, EQueueMode::Spsc>;
    NoiseCalculator *NoiseCalculator = new class NoiseCalculator(chunkActor, loc);
    NoiseCalculator->EnsureCompletion();
    chunkActor->updateMesh();
    NoiseCalculator->chunkloc = loc;
    NoiseCalculator->genchunk = chunkActor;
    
    CalcQueue->Enqueue(*NoiseCalculator);
    FPlatformProcess::Sleep(sleep_seconds_first);
    if(CalcQueue->Dequeue(*NoiseCalculator)){
        FPlatformProcess::WaitForProc(FProcHandle &ProcessHandle);
        FPlatformProcess::Sleep(sleep_seconds_sec);
        chunkActor->updateMesh();
    }else{
        FPlatformProcess::Sleep(0.01f);
        chunkActor->updateMesh();
    }
    delete NoiseCalculator;
    return chunkActor;
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
    Super::BeginPlay();
    FVector loc = FVector(0.0f, 0.0f, 10.0f);
    loc.Z = -25.0f;
    FRotator Rotation(0.0f, 0.0f, 0.0f);
    FActorSpawnParameters SpawnInfo;
    FVector loc = FVector(0.0f, 0.0f, 0.0f);
    for(int i = 0; i <= 800; i++){
        ATerrainGenManager* chunkActor = Cast<ATerrainGenManager>(GetWorld()->SpawnActor<ATerrainGenManager>(loc, Rotation, SpawnInfo));
        chunkActor->initialize(loc.X / 200.0f, loc.Y / 200.0f, -25.0f);
        chunkActor->SetGenLoc(loc.X / 200.0f, loc.Y / 200.0f, -25.0f);
        chunkActor->TerrainMaterial = TerrainMaterial;
        NoiseCalculator* NoiseCalculator = new class NoiseCalculator(chunkActor);
        chunkActor->updateMesh();
        loc.X += 5.0f;
        multi_chunkgen(FVector(i * 50.0f, 0.0f, 0.0f));
    }
    multi_chunkgen(loc);
}
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

