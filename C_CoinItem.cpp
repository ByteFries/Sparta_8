// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CoinItem.h"

#include "../GameMode/C_GameState.h"

AC_CoinItem::AC_CoinItem()
{
	
}

void AC_CoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (UWorld* World = GetWorld())
	{
		if (AC_GameState* GameState = World->GetGameState<AC_GameState>())
		{
			GameState->AddScore(PointValue);
			GameState->OnCoinCollected();
			DestroyItem();
		}
	}
	
}
