// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SUnrealObjectClass.h"
#include "Example/SPigeon.h"
#include "Example/SFlyable.h"
#include "JsonObjectConverter.h"
#include "UObject/SavePackage.h"


USGameInstance::USGameInstance()
{
}

void USGameInstance::Init()
{
	Super::Init();

	// 데이터 테이블 가져왔을 때 올바른지 아닌지 판단하는 코드
	if(false == ::IsValid(CharacterStatDataTable) || CharacterStatDataTable->GetRowMap().Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough data in CharacterStatDataTable."));
	}
	else
	{
		for(int32 i = 1; i <= CharacterStatDataTable->GetRowMap().Num(); ++i)
		{
			check(nullptr != GetCharacterStatDataTableRow(i));
		}
	}
}

void USGameInstance::Shutdown()
{
	Super::Shutdown();
}

FSStatTableRow* USGameInstance::GetCharacterStatDataTableRow(int32 InLevel)
{
	if(true == ::IsValid(CharacterStatDataTable))
	{
		return CharacterStatDataTable->FindRow<FSStatTableRow>(*FString::FromInt(InLevel), TEXT(""));
	}

	return nullptr;
}
