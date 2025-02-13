// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_ItemBase.h"
#include "C_Mine.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AC_Mine : public AC_ItemBase
{
	GENERATED_BODY()

public:
	AC_Mine();

	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem() override;
	void Explode();

	FTimerHandle TimerHandle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float ExplosionDelay;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float ExplosionRadius;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int Damage;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Effects")
	UParticleSystem* ExplosionPickupParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Effects")
	USoundBase* ExplosionPickupSound;
	
	USphereComponent* ExplosionSphere;

	bool bHasExploded = false;
};
