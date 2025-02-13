// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_ItemBase.h"
#include "C_CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AC_CoinItem : public AC_ItemBase
{
	GENERATED_BODY()

public:
	AC_CoinItem();

	virtual void ActivateItem(AActor* Activator) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 PointValue;
	
};
