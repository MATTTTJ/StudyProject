// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SPlayerController.h"
#include "UI/SHUD.h"
#include "Game/SPlayerState.h"
#include "Component/SStatComponent.h"
#include "Characters/SRPGCharacter.h"
#include "Blueprint/UserWidget.h"

ASPlayerController::ASPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASPlayerController::ToggleMenu()
{
	if(false == bIsMenuOn)
	{
		MenuUIInstance->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(MenuUIInstance->GetCachedWidget());

		SetPause(true);

		SetInputMode(Mode);
		bShowMouseCursor = true;
	}
	else
	{
		MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);

		SetPause(false);
		bShowMouseCursor = false;
	}

	bIsMenuOn = !bIsMenuOn;
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if(true == ::IsValid(HUDWidgetClass))
	{
		HUDWidget = CreateWidget<USHUD>(this, HUDWidgetClass);
		if(true == ::IsValid(HUDWidget))
		{
			HUDWidget->AddToViewport();

			ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
			if(true == ::IsValid(SPlayerState))
			{
				HUDWidget->BindPlayerState(SPlayerState);
			}

			ASCharacter* PC = GetPawn<ASCharacter>();
			if(true == ::IsValid(PC))
			{
				USStatComponent* StatComponent = PC->GetStatComponent();
				if(true == ::IsValid(StatComponent))
				{
					HUDWidget->BindStatComponent(StatComponent);
				}
			}
		}
	}

	if(true == ::IsValid(MenuUIClass))
	{
		MenuUIInstance = CreateWidget<UUserWidget>(this, MenuUIClass);
		if(true == ::IsValid(MenuUIInstance))
		{
			MenuUIInstance->AddToViewport(3); // »óÀ§¿¡ ¶ç¿ò

			MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(true == ::IsValid(CrosshairUIClass))
	{
		UUserWidget* CrosshairUI = CreateWidget<UUserWidget>(this, CrosshairUIClass);
		if(true ==::IsValid(CrosshairUI))
		{
			CrosshairUI->AddToViewport(1);

			CrosshairUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

