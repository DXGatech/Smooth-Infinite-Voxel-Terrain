// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TerrainGenManager.h"

/**
 * 
 */
class POLYVOX_TEST_API NoiseCalculator : FRunnable
{
public:
    
    NoiseCalculator* calc;
    
    FRunnableThread* Thread;
    
    ATerrainGenManager* genchunk;
    
    float noiseVal = 0.0f;
    
    float de_frequency = 0.005;
    
    float amplitude = 15.0f;
    
    float x = 0.0f;
    
    bool isThreadFinished = false;
    
    FVector chunkloc;
    
    float y = 0.0f;
    
    float z = 0.0f;
    
    bool isFinished() const
    {
        return noiseVal == 0.0f;
    }
    
    NoiseCalculator(ATerrainGenManager* chunk, FVector loc);
    NoiseCalculator();
	~NoiseCalculator();
    
    virtual bool Init();
    virtual void Stop();
    virtual uint32 Run();
    //virtual void Stop();
    
    static void ShutDown();
    
    void EnsureCompletion();

};
