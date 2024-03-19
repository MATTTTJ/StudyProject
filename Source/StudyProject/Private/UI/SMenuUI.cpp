// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SMenuUI.h"
#include "Components/Button.h"
#include "Controllers/SPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void USMenuUI::NativeConstruct()
{
	ResumeGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnResumeGameButtonClicked);
	ReturnTitleButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnTitleButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
}

void USMenuUI::OnResumeGameButtonClicked()
{
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnResumeGameButtonClicked() has been called.")));
	ASPlayerController* PlayerController = Cast<ASPlayerController>(GetOwningPlayer());
	if(true == ::IsValid(PlayerController))
	{
		PlayerController->ToggleMenu();
	}
}

void USMenuUI::OnReturnTitleButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Title")));
}

void USMenuUI::OnExitGameButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
