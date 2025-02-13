// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"

#include "../Input/C_PlayerController.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "OpenWorldRPG/GameMode/C_GameState.h"

AC_PlayerCharacter::AC_PlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	Health = MaxHealth = 100;	
}

void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateWidget();
}

void AC_PlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AC_PlayerController* PlayerController = Cast<AC_PlayerController>(GetController()))
		{
			EnhancedInputComponent->BindAction(
			PlayerController->MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AC_PlayerCharacter::Move
			);

			EnhancedInputComponent->BindAction(
			PlayerController->LookAction,
			ETriggerEvent::Triggered,
			this,
			&AC_PlayerCharacter::Look
			);
		}
	}
}

void AC_PlayerCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health+Amount, 0.0f, MaxHealth);

	UpdateWidget();
}

void AC_PlayerCharacter::OnDeath()
{
	if (AC_GameState* GameState = GetWorld() ? GetWorld()->GetGameState<AC_GameState>() : nullptr)
	{
		GameState->OnGameOver();
	}
}

void AC_PlayerCharacter::UpdateWidget()
{
	if (!WidgetComponent) return;

	UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();

	if (!Widget) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), Health, MaxHealth)));
	}
}

void AC_PlayerCharacter::WidgetInvisible(bool val)
{
	WidgetComponent->SetVisibility(val);
}

void AC_PlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X)) AddMovementInput(GetActorForwardVector(), MoveInput.X);
	if (!FMath::IsNearlyZero(MoveInput.Y)) AddMovementInput(GetActorRightVector(), MoveInput.Y);
}

void AC_PlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

float AC_PlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.f, MaxHealth);
	UpdateWidget();
	if (Health <= 0.f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}


