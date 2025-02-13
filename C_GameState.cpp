#include "C_GameState.h"

#include "C_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../C_SpawnVolume.h"
#include "../Item/C_CoinItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "OpenWorldRPG/Input/C_PlayerController.h"

AC_GameState::AC_GameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f; // 한 레벨당 30초
	CurrentLevelIndex = 0;
	MaxLevels = 3;

	WaveDelay = 30.f;
	WaveIndex = -1;
}

void AC_GameState::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 첫 레벨부터 진행

	if (UC_GameInstance* GameInstance = Cast<UC_GameInstance>(GetGameInstance()))
	{
		//StartLevel();

		if (GameInstance->CurrentLevelIndex == -1)
		{
			
		}
		else
		{
			if (!GameInstance->bIsGameStarted)
			{
				GameStart();
			}
			StartWave();
		}
		
	}
	
	GetWorldTimerManager().SetTimer(
		HUDTimerHandle,
		this,
		&AC_GameState::UpdateHUD,
		0.1,
		true
		);
}

void AC_GameState::GameStart()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AC_PlayerController* C_PlayerController = Cast<AC_PlayerController>(PlayerController))
		{
			C_PlayerController->ShowGameHUD();
		}
	}
	
	if (UC_GameInstance* GameInstance = Cast<UC_GameInstance>(GetGameInstance()))
	{
		GameInstance->bIsGameStarted = true;
	}

}


void AC_GameState::StartWave()
{	
	WaveIndex += 1;
	if (WaveIndex == 3)
	{
		OnGameOver();
		return;
	}
	DeleteRemainItems();	
	SpawnItems();
	
	GetWorldTimerManager().SetTimer(
	WaveTimerHandle,
	this,
	&AC_GameState::StartWave,
	WaveDelay,
	false
	);
}

void AC_GameState::DeleteRemainItems() const
{
	TArray<AActor*> FoundItems;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_ItemBase::StaticClass(), FoundItems);

	for (const auto& BaseItem : FoundItems)
	{
		if (const auto& Item = Cast<AC_ItemBase>(BaseItem))
		{
			Item->DestroyItem();
		}
	}
}

void AC_GameState::SpawnItems()
{
	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_SpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.IsEmpty()) return;
	AC_SpawnVolume* Volume = Cast<AC_SpawnVolume>(FoundVolumes[0]);
	Volume->ItemDataTable = DataTables[WaveIndex];
	
	constexpr int32 ItemToSpawn = 40;
	
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			if (AC_SpawnVolume* SpawnVolume = Cast<AC_SpawnVolume>(FoundVolumes[0]))
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(AC_CoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	
}

int32 AC_GameState::GetScore() const
{
	return Score;
}

void AC_GameState::AddScore(int32 Amount)
{
	Score += Amount;
	auto GameInstance = Cast<UC_GameInstance>(GetGameInstance());

	if (GameInstance != nullptr)
	{
		GameInstance->AddToScore(Amount);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void AC_GameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AC_PlayerController* C_PlayerController = Cast<AC_PlayerController>(PlayerController))
		{
			C_PlayerController->ShowGameHUD();
		}
	}
	
	auto GameInstance = Cast<UC_GameInstance>(GetGameInstance());

	if (GameInstance != nullptr)
	{
		CurrentLevelIndex = GameInstance->CurrentLevelIndex;
	}
	else
	{
		CurrentLevelIndex = 0;
	}
	
	// 레벨 시작 시, 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	SpawnItems();
}

void AC_GameState::OnLevelTimeUp()
{
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AC_GameState::OnCoinCollected()
{
	CollectedCoinCount++;
	
	// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void AC_GameState::EndLevel()
{
	// 타이머 해제
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// 다음 레벨 인덱스로
	CurrentLevelIndex++;
	
	auto GameInstance = Cast<UC_GameInstance>(GetGameInstance());

	if (GameInstance != nullptr)
	{
		GameInstance->CurrentLevelIndex = CurrentLevelIndex;
	}
	
	// 모든 레벨을 다 돌았다면 게임 오버 처리
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	// 레벨 맵 이름이 있다면 해당 맵 불러오기
	//if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	//{
	//UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	//}
	//else
	{
		// 맵 이름이 없으면 게임오버
		//OnGameOver();
	}
}

void AC_GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		AC_PlayerController* SpartaPlayerController = Cast<AC_PlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UC_GameInstance* C_GameInstance = Cast<UC_GameInstance>(GameInstance);
						if (C_GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), C_GameInstance->TotalScore)));
						}
					}
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), WaveIndex + 1)));
				}
			}
		}
	}
}

void AC_GameState::OnGameOver()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AC_PlayerController* C_PlayerController = Cast<AC_PlayerController>(PlayerController))
		{
			C_PlayerController->SetPause(true);
			C_PlayerController->ShowGameOverMenu();
		}
	}
}
