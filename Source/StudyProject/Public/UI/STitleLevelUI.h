// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STitleLevelUI.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USTitleLevelUI : public UUserWidget
{
	GENERATED_BODY()

public:
	USTitleLevelUI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnNewGameButtonClicked();

	UFUNCTION()
	void OnExitGameButtonClicked();

	UFUNCTION()
	void OnSavedGameButtonClicked();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> NewGameButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ExitGameButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleWidget, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UButton> SavedGameButton;
};
