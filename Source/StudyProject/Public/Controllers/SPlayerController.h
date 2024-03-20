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

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> MenuUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ASPlayerController, Meta = (AllowPrivateAccess))
	TObjectPtr<class UUserWidget> MenuUIInstance;

	bool bIsMenuOn = false;

private:
	UPROPERTY()
	TObjectPtr<class USHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class USHUD> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPlayerController", Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> CrosshairUIClass;

};
