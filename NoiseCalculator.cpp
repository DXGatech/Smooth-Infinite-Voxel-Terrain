// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyVox_Test.h"
//#include "TerrainGenManager.h"
#include "SimplexNoiseBPLibrary.h"
#include "NoiseCalculator.h"

NoiseCalculator::NoiseCalculator(ATerrainGenManager* chunk, FVector loc)
{
    genchunk = chunk;
    chunkloc = loc;
    Thread = FRunnableThread::Create(this, TEXT("NoiseCalculator"), 0, TPri_BelowNormal);
}

NoiseCalculator::NoiseCalculator()
{
    Thread = FRunnableThread::Create(this, TEXT("NoiseCalculator"), 0, TPri_BelowNormal);
}

NoiseCalculator::~NoiseCalculator()
{
    delete Thread;
    Thread = NULL;
}

bool NoiseCalculator::Init()
{
    return true;
}

void NoiseCalculator::Stop()
{
    isThreadFinished = true;
}

uint32 NoiseCalculator::Run()
{
    UE_LOG(LogTemp, Error, TEXT("Spawning chunk"))
    FVector loc = genchunk->GetActorLocation();
    FVector loc = FVector(0.0f, 0.0f, -10.0f);
    genchunk->initialize(chunkloc.X / 200.0f, chunkloc.Y / 200.0f, -10.0f);
    genchunk->SetGenLoc(chunkloc.X / 200.0f, chunkloc.Y / 200.0f, -10.0f);
    isThreadFinished = true;
    return 0;
}

void NoiseCalculator::EnsureCompletion()
{
    Stop();
    Thread->WaitForCompletion();
}

void NoiseCalculator::ShutDown()
{
}













