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
	UE_LOG(LogTemp, Log, TEXT("USGameInstance() has been Called"));

	Name = TEXT("USGameInstance Class Default Object");
	// CDO 의 Name 속성이 저장됨
	// 중단점을 걸어보면 언리얼 에디터가 실행되기 전에 호출됨을 알 수 있음.
}

void USGameInstance::Init()
{
	Super::Init();

	
}

void USGameInstance::Shutdown()
{

	Super::Shutdown();
}
