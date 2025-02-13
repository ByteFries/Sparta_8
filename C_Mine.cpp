// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Mine.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AC_Mine::AC_Mine()
{
	ExplosionDelay = 1.0f;
	ExplosionRadius = 300.f;
	Damage = 30.f;
	ItemType = "Mine";

	ExplosionSphere = CreateDefaultSubobject<USphereComponent>("MineSphere");
	ExplosionSphere->InitSphereRadius(ExplosionRadius);
	ExplosionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionSphere->SetupAttachment(Scene);

	bHasExploded = false;
}

void AC_Mine::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;

	bHasExploded = true;
	
	Super::ActivateItem(Activator);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_Mine::Explode, ExplosionDelay, false);
}

void AC_Mine::DestroyItem()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	Super::DestroyItem();
}

void AC_Mine::Explode()
{
	//UParticleSystemComponent* Particle;
	
	//if (ExplosionPickupParticle)
	{
		//Particle = UGameplayStatics::SpawnEmitterAtLocation(
		//	GetWorld(),
		//	ExplosionPickupParticle,
		//	GetActorLocation(),
		//	GetActorRotation(),
		//	false
		//	);

		//if (Particle)
		//{
		//	FTimerHandle ParticleTimerHandle;
//
		//	GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle,
		//		[Particle]()
		//		{
		//			Particle->DestroyComponent();
		//		}, 1.0f, false);
		//}
	}

	//if (ExplosionPickupSound)
	{
		//UGameplayStatics::PlaySoundAtLocation(
		//GetWorld(),
		//ExplosionPickupSound,
		//GetActorLocation(),
		//GetActorRotation(),
		//true
		//);
	}
	
	TArray<AActor*> OverlappingActors;
	ExplosionSphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				Damage,
				nullptr,
				this,
				UDamageType::StaticClass()
				);
		}
	}
	
	DestroyItem();
}
