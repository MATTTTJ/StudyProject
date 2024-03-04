// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameModeBase.h"
#include "Characters/SPlayerPawn.h"
#include "Controllers/SPlayerController.h"

ASGameModeBase::ASGameModeBase()
{
	PlayerControllerClass = ASPlayerController::StaticClass();
}
