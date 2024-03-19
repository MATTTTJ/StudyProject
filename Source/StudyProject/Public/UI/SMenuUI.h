// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USMenuUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeGameButtonClicked();

	UFUNCTION()
	void OnReturnTitleButtonClicked();

	UFUNCTION()
	void OnExitGameButtonClicked();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMenuUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ResumeGameButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMenuUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ReturnTitleButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USMenuUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ExitGameButton;
	
	
};
