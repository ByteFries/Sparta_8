// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "OpenWorldRPG/Character/C_PlayerCharacter.h"
#include "OpenWorldRPG/GameMode/C_GameInstance.h"
#include "OpenWorldRPG/GameMode/C_GameState.h"

AC_PlayerController::AC_PlayerController()
{
}

void AC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();

	if (CurrentMapName.Contains("L_MainMenu"))
	{
		ShowMainMenu(false);
	}
}

void AC_PlayerController::ClearAllWidget()
{
	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}
	
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	if (GameOverMenuWidget)
	{
		GameOverMenuWidget->RemoveFromParent();
		GameOverMenuWidget = nullptr;
	}
}

UUserWidget* AC_PlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AC_PlayerController::ShowGameHUD()
{
	ClearAllWidget();

	if (!HUDWidgetClass) return;
	
	HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	
	if (AC_GameState* GameState = GetWorld() ? GetWorld()->GetGameState<AC_GameState>() : nullptr)
	{
		GameState->UpdateHUD();
	}
	
	if (AC_PlayerCharacter* C_Character = Cast<AC_PlayerCharacter>(GetPawn()))
	{
		C_Character->WidgetInvisible(true);
	}
}

void AC_PlayerController::ShowMainMenu(bool bIsRestart)
{
	ClearAllWidget();
	
	if (!MainMenuWidgetClass) return;
	
	MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	if (MainMenuWidget == nullptr) return;

	MainMenuWidget->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	if (AC_PlayerCharacter* C_Character = Cast<AC_PlayerCharacter>(GetPawn()))
	{
		C_Character->WidgetInvisible(false);
	}
	
	if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidget->GetWidgetFromName(TEXT("StartButtonText"))))
	{
		ButtonText->SetText(bIsRestart ? FText::FromString(TEXT("Restart")) : FText::FromString(TEXT("Start")));
	}
	
	if (bIsRestart)
	{
		
		if (UFunction* PlayerAnimFunc = MainMenuWidget->FindFunction(FName("PlayGameOverAnim")))
		{
			MainMenuWidget->ProcessEvent(PlayerAnimFunc, nullptr);
		}
		
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidget->GetWidgetFromName("TotalScore")))
		{
			if (UC_GameInstance* GameInstance = Cast<UC_GameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), GameInstance->TotalScore)));
			}
		}
	}
}

void AC_PlayerController::ShowGameOverMenu()
{
	ClearAllWidget();

	if (!GameOverMenuWidgetClass) return;

	GameOverMenuWidget = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);
	if (!GameOverMenuWidget) return;
	
	GameOverMenuWidget->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void AC_PlayerController::StartGame()
{
	if (UC_GameInstance* GameInstance = Cast<UC_GameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->CurrentLevelIndex = 0;
		GameInstance->TotalScore = 0;
		GameInstance->bIsGameStarted = false;
	}
	
	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}

void AC_PlayerController::MoveToMainMenu()
{
	if (UC_GameInstance* GameInstance = Cast<UC_GameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->bIsGameStarted = false;
		GameInstance->CurrentLevelIndex = -1;
	}

	ShowMainMenu(true);
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("L_MainMenu"));
}
