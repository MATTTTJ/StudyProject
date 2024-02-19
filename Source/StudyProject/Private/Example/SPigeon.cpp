// Fill out your copyright notice in the Description page of Project Settings.


#include "Example/SPigeon.h"


USPigeon::USPigeon()
{
	Name = TEXT("Pigeon");
	ID = 0;
}

void USPigeon::Fly()
{
	UE_LOG(LogTemp, Log, TEXT("%s is now flying."), *Name);
}

void USPigeon::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar << Name;
	Ar << ID;
}
