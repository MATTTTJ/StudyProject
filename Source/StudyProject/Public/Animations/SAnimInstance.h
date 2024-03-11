// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend class ASRPGCharacter;

public:
	USAnimInstance();
	// AnimInstance ÀÇ Tick ÇÔ¼ö
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;

private:
	void PlayAttackAnimMontage();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USAnimInstance")
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USAnimInstance")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USAnimInstance")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USAnimInstance")
	uint8 bIsCrouching : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "USAnimInstance")
	TObjectPtr<class UAnimMontage> AttackAnimMontage;
};
