// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UC_ItemInterface : public UInterface
{
	GENERATED_BODY()
};

class OPENWORLDRPG_API IC_ItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OverlapActor,
	UPrimitiveComponent* OverlapComp,
	int32 OtherActorIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	) = 0;

	UFUNCTION()
	virtual void OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OverlapActor,
	UPrimitiveComponent* OverlapComp,
	int32 OtherActorIndex
	) = 0;
	
	virtual void ActivateItem(AActor* Activator) = 0; //정의
	
	virtual FName GetItemType() const = 0; //string 보다 가볍다
};
