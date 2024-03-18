// SLoadingPlayerController.cpp


#include "Controllers/SLoadingPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ASLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if(true == ::IsValid(GM))
	{
		FString NextLevelString = UGameplayStatics::ParseOption(GM->OptionsString, FString(TEXT("NextLevel")));
		FString SavedTypeString = UGameplayStatics::ParseOption(GM->OptionsString, FString(TEXT("Saved")));
		UGameplayStatics::OpenLevel(GM, *NextLevelString, false, *SavedTypeString);
	}
}
