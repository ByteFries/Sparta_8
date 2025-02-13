// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UC_GameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 TotalScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 CurrentWave;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	bool bIsGameStarted;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

};