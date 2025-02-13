// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpawnVolume.h"

#include "Item/Spawn/C_ItemSpawnRow.h"
#include "Components/BoxComponent.h"

// Sets default values
AC_SpawnVolume::AC_SpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

AActor* AC_SpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

FVector AC_SpawnVolume::GetRandomPointVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent(); //박스 크기의	반지름
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
		);
}

AActor* AC_SpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	auto SpawnedItem = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointVolume(),
		FRotator::ZeroRotator
		);

	return SpawnedItem;
}

FItemSpawnRow* AC_SpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;

	static const FString ContextString = TEXT("ItemSpawnFailed");
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;

	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandVal = FMath::FRandRange(0, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandVal <= AccumulateChance)
			return Row;
	}
	return nullptr;	
}