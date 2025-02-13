// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_CoinItem.h"
#include "C_BigCoinItem.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AC_BigCoinItem : public AC_CoinItem
{
	GENERATED_BODY()
public:
	AC_BigCoinItem();

	virtual void ActivateItem(AActor* Activator) override;
	
};
