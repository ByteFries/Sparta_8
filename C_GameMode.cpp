// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GameMode.h"

#include "C_GameState.h"
#include "OpenWorldRPG/Character/C_PlayerCharacter.h"
#include "OpenWorldRPG/Input/C_PlayerController.h"

AC_GameMode::AC_GameMode()
{
	DefaultPawnClass = AC_PlayerCharacter::StaticClass();
	PlayerControllerClass = AC_PlayerController::StaticClass();
	GameStateClass = AC_GameState::StaticClass();
}
