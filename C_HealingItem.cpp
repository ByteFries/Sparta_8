// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HealingItem.h"

#include "../Character/C_PlayerCharacter.h"

AC_HealingItem::AC_HealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AC_HealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AC_PlayerCharacter* Player = Cast<AC_PlayerCharacter>(Activator))
		{
			Player->AddHealth(HealAmount);
			DestroyItem();
		}
	}
	
}
