// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GameInstance.h"

UC_GameInstance::UC_GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = -1;
	CurrentWave = -1;
	bIsGameStarted = false;
}

void UC_GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}
