// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    ASCharacter();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", meta = (AllowPrivateAccess))
    TObjectPtr<class USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", meta = (AllowPrivateAccess))
    TObjectPtr<class UCameraComponent> CameraComponent;
};