// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BigCoinItem.h"

AC_BigCoinItem::AC_BigCoinItem()
{
	PointValue = 50;
	ItemType = "Big Coin";
}

void AC_BigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
