// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SmallCoinItem.h"

AC_SmallCoinItem::AC_SmallCoinItem()
{
	PointValue = 10;
	ItemType = "SmallCoin";
}

void AC_SmallCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
