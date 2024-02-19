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

	FBirdData SrcRawData(TEXT("Pigeon17"), 17);
	UE_LOG(LogTemp, Log, TEXT("[SrcRawData] Name : %s, ID : %d"), *SrcRawData.Name, SrcRawData.ID);

	const FString SavedDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	UE_LOG(LogTemp, Log, TEXT("SavedDir: %s"), *SavedDir);

	const FString RawDataFileName(TEXT("RawData.bin"));
	FString AbsolutePathForRawData = FPaths::Combine(*SavedDir, *RawDataFileName);
	UE_LOG(LogTemp, Log, TEXT("Relative path for Saved file : %s"), *AbsolutePathForRawData);
	FPaths::MakeStandardFilename(AbsolutePathForRawData);
	UE_LOG(LogTemp, Log, TEXT("Absolue path for Saved file: %s"), *AbsolutePathForRawData);

	FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*AbsolutePathForRawData);
	if(nullptr != RawFileWriterAr)
	{
		*RawFileWriterAr << SrcRawData;
		RawFileWriterAr->Close();
		delete RawFileWriterAr;
		RawFileWriterAr = nullptr;
	}

	FBirdData DstRawData;
	FArchive* RawFileReadAr = IFileManager::Get().CreateFileReader(*AbsolutePathForRawData);
	if(nullptr != RawFileReadAr)
	{
		*RawFileReadAr << DstRawData;
		RawFileReadAr->Close();
		delete RawFileReadAr;
		RawFileReadAr = nullptr;

		UE_LOG(LogTemp, Log, TEXT("[DstRawdata] Name: %s, ID: %d"), *DstRawData.Name, DstRawData.ID);
	}

	SerializedPigeon = NewObject<USPigeon>();
	SerializedPigeon->SetName(TEXT("Pigeon76"));
	SerializedPigeon->SetID(76);
	UE_LOG(LogTemp, Log, TEXT("[SerializedPigeon] Name: %s, ID: %d"), *SerializedPigeon.GetName(), SerializedPigeon->GetID());

	const FString ObjectDataFileName(TEXT("ObjectData.bin"));
	FString AbsolutePathForObjectData = FPaths::Combine(*SavedDir, *ObjectDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForObjectData);

	TArray<uint8> BufferArray;
	FMemoryWriter MemoryWriterAr(BufferArray);
	SerializedPigeon->Serialize(MemoryWriterAr);

	TUniquePtr<FArchive> ObjectDataFileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*AbsolutePathForObjectData));
	if(nullptr != ObjectDataFileWriterAr)
	{
		*ObjectDataFileWriterAr << BufferArray;
		ObjectDataFileWriterAr->Close();

		ObjectDataFileWriterAr = nullptr;
	}


	TArray<uint8> BufferArrayFromObjectDataFile;
	TUniquePtr<FArchive> ObjectDataFileReadAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*AbsolutePathForObjectData));
	if(nullptr != ObjectDataFileReadAr)
	{
		*ObjectDataFileReadAr << BufferArrayFromObjectDataFile;
		ObjectDataFileReadAr->Close();

		ObjectDataFileReadAr = nullptr;
	}

	FMemoryReader MemoryReaderAr(BufferArrayFromObjectDataFile);
	USPigeon* Pigeon77 = NewObject<USPigeon>();
	Pigeon77->Serialize(MemoryReaderAr);
	UE_LOG(LogTemp, Log, TEXT("[Pigeon77] Name: %s, ID: %d"), *Pigeon77->GetName(), Pigeon77->GetID());

	const FString JsonDataFileName(TEXT("StudyJsonFile.txt"));
	FString AbsolutePathForJsonData = FPaths::Combine(*SavedDir, *JsonDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForJsonData);

	TSharedRef<FJsonObject> SrcJsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(SerializedPigeon->GetClass(), SerializedPigeon, SrcJsonObject);

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriterAr = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
	if (true == FJsonSerializer::Serialize(SrcJsonObject, JsonWriterAr))
	{
		FFileHelper::SaveStringToFile(JsonOutString, *AbsolutePathForJsonData);
	}

	FString JsonInString;
	FFileHelper::LoadFileToString(JsonInString, *AbsolutePathForJsonData);
	// Json 오브젝트는 안전하게 관리하기 위해 Shared 포인터를 사용한다.
	TSharedRef<TJsonReader<TCHAR>> JsonReaderAr = TJsonReaderFactory<TCHAR>::Create(JsonInString);
	

	TSharedPtr<FJsonObject> DstJsonObject;
	if (true == FJsonSerializer::Deserialize(JsonReaderAr, DstJsonObject))
	{
		USPigeon* Pigeon78 = NewObject<USPigeon>();
		if (true == FJsonObjectConverter::JsonObjectToUStruct(DstJsonObject.ToSharedRef(), Pigeon78->GetClass(), Pigeon78))
		{
			UE_LOG(LogTemp, Log, TEXT("[Pigeon78] Name: %s, ID: %d"), *Pigeon78->GetName(), Pigeon78->GetID());
		}
	}
}

void USGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("Shutdown() has been Called"));

	Super::Shutdown();
}
