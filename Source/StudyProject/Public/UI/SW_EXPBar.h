// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_Bar.h"
#include "SW_EXPBar.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USW_EXPBar : public USW_Bar
{
	GENERATED_BODY()


public:
	void SetMaxEXP(float InMaxEXP);

	UFUNCTION()
	void OnCurrentEXPChange(float InOldEXP, float InNewEXP);

	virtual void InitializeEXPBarWidget(class ASPlayerState* NewPlayerState);
	
};
