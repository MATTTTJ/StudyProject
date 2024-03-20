// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "STPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USTPSAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	TObjectPtr<class ASTPSCharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	FVector MoveInputWithMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	FVector MoveInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USTPSAnimInstance", Meta = (AllowPrivateAccess))
	FRotator ControlRotation;

	
};
