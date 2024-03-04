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
	// CDO �� Name �Ӽ��� �����
	// �ߴ����� �ɾ�� �𸮾� �����Ͱ� ����Ǳ� ���� ȣ����� �� �� ����.
}

void USGameInstance::Init()
{
	Super::Init();

	
}

void USGameInstance::Shutdown()
{

	Super::Shutdown();
}
