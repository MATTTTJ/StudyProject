// ASPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ASPlayerController();

	void ToggleMenu();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnOwningCharacterDead();

	UFUNCTION(Client, Reliable)
	void ShowWinnerUI();

	UFUNCTION(Client, Reliable)
	void ShowLooserUI(int32 InRanking);

	UFUNCTION(Client, Reliable)
	void ReturnToLobby();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	FText NotificationText;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> MenuUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TObjectPtr<class UUserWidget> MenuUIInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> NotificationTextUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> WinnerUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> LooserUIClass;

	bool bIsMenuOn = false;

private:
	UPROPERTY()
	TObjectPtr<class USHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class USHUD> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> CrosshairUIClass;

};
