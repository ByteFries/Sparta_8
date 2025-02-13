// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_ItemInterface.h"
#include "C_ItemBase.generated.h"

class USphereComponent;
UCLASS()
class OPENWORLDRPG_API AC_ItemBase : public AActor, public IC_ItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_ItemBase();

	virtual void DestroyItem();
	
protected:
	virtual void OnItemOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OverlapActor,
	UPrimitiveComponent* OverlapComp,
	int32 OtherActorIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	) override;
	
	virtual void OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OverlapActor,
	UPrimitiveComponent* OverlapComp,
	int32 OtherActorIndex
	) override;
	
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Component")
	USceneComponent* Scene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Component")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Component")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Effects")
	UParticleSystem* PickupParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Effects")
	USoundBase* PickupSound;

	FTimerHandle ParticleTimerHandle;
};
