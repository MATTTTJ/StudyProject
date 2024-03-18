// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameModeBase.h"
#include "Characters/SPlayerPawn.h"
#include "Controllers/SPlayerController.h"
#include "Game/SPlayerState.h"

ASGameModeBase::ASGameModeBase()
{
	PlayerControllerClass = ASPlayerController::StaticClass();
	DefaultPawnClass = ASPlayerPawn::StaticClass();
}

void ASGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();

	if(true == ::IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}
}
