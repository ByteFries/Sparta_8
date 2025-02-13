// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_ItemBase.h"
#include "C_HealingItem.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AC_HealingItem : public AC_ItemBase
{
	GENERATED_BODY()

public:
	AC_HealingItem();

	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int HealAmount;
};
