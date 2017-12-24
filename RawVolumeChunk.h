// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PolyVox/RawVolume.h"
#include "Engine.h"
#include "Header.h"
#include "ProceduralMeshComponent.h"
#include "PolyVox/MaterialDensityPair.h"
#include "RawVolumeChunk.generated.h"

UCLASS()
class POLYVOX_TEST_API ARawVolumeChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    ARawVolumeChunk();
    
    void createBlockInVolume(PolyVox::RawVolume<uint8_t>& volData, float fRadius);
    
    void createSphereInVolume(PolyVox::RawVolume<uint8_t>& volData, float fRadius);
    
    void createHeightmapInVolume(PolyVox::RawVolume<uint8_t>& volData, float fRadius);
    
    UPROPERTY(EditAnywhere)
    UProceduralMeshComponent* Mesh = nullptr;
    
    PolyVox::RawVolume<uint8_t> RawVol();
    
    TSharedPtr<PolyVox::RawVolume<uint8_t>> volData;
    
    UFUNCTION(BlueprintCallable)
    void ModifyChunk(FVector WorldLoc);
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
