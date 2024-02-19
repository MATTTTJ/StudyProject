// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USGameInstance();

public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TObjectPtr<class USPigeon> SerializedPigeon;
};
