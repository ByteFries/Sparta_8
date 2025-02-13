// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ItemBase.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AC_ItemBase::AC_ItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
	SphereComponent->SetupAttachment(Scene);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(SphereComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_ItemBase::OnItemOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AC_ItemBase::OnItemEndOverlap);
}

void AC_ItemBase::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OverlapActor,
	UPrimitiveComponent* OverlapComp, int32 OtherActorIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		ActivateItem(OverlapActor);
	}
}

void AC_ItemBase::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OverlapActor,
	UPrimitiveComponent* OverlapComp, int32 OtherActorIndex)
{
}

void AC_ItemBase::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle;
	
	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
			);

		if (Particle != nullptr)
		{
			

			GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle,
				[Particle]()
				{
					Particle->DestroyComponent();
				}, 0.95f, false);
		}
	}
	
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation(),
			GetActorRotation(),
			true
			);
	}
}

FName AC_ItemBase::GetItemType() const
{
	return ItemType;
}

void AC_ItemBase::DestroyItem()
{
	GetWorld()->GetTimerManager().ClearTimer(ParticleTimerHandle);
	Destroy();
}

