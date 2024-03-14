// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "SGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSStatTableRow()
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxEXP;
};

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

	const UDataTable* GetCharacterStatDataTable() { return CharacterStatDataTable; }

	// 줄 수 넣으면 그 열의 값을 가져옴(지금은 레벨을 가져온다고 보면된다.
	FSStatTableRow* GetCharacterStatDataTableRow(int32 InLevel);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USGameInstance", Meta = (AllowPrivateAccess))
	class UDataTable* CharacterStatDataTable;
};
