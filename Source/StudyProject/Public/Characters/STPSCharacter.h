// STPSCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/SCharacter.h"
#include "STPSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASTPSCharacter : public ASCharacter
{
	GENERATED_BODY()

public:
	ASTPSCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	float GetForwardInputValue() const { return ForwardInputValue; }

	float GetRightInputValue() const { return RightInputValue; }

	float GetCurrentAimPitch() const { return CurrentAimPitch; }

	float GetCurrentAimYaw() const { return CurrentAimYaw; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void Attack(const FInputActionValue& InValue);

	void Fire();

	void ToggleTrigger(const FInputActionValue& InValue);

	void StartFire(const FInputActionValue& InValue);

	void StopFire(const FInputActionValue& InValue);

	void StartIronSight(const FInputActionValue& InValue);

	void EndInronSight(const FInputActionValue& InValue);

	void SpawnLandMine(const FInputActionValue& InValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnLandMine_Server();

	UFUNCTION()
	void OnHittedRagdollRestoreTimerElapsed();

	UFUNCTION(Server, Unreliable) // ÇÑ µÎ¹ø Á¤µµ´Â ¾ÃÇôµµ ±¦Âú´Ù.
	void UpdateInputValue_Server(const float& InForwardInputValue, const float& InRightInputValue);

	UFUNCTION(Server, Unreliable)
	void UpdateAimValue_Server(const float& InAimPitchValue, const float& InAimYawValue);

	UFUNCTION(Server, Unreliable)
	void PlayAttackMontage_Server();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayAttackMontage_NetMulticast();

	UFUNCTION(Server, Reliable)
	void ApplyDamageAndDrawLine_Server(const FVector& InDrawStart,
		const FVector& InDrawEnd,
		class ACharacter* InHittedCharacter,
		float InDamage,
		struct FDamageEvent const& InDamageEvent,
		AController* InEventInstigator, AActor* InDamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void DrawLine_NetMulticast(const FVector& InDrawStart, const FVector& InDrawEnd);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class USInputConfigData> PlayerCharacterInputConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> PlayerCharacterInputMappingContext;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	float ForwardInputValue;

	float PreviousForwardInputValue = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	float RightInputValue;

	float PreviousRightInputValue = 0.f;


	float TargetFOV = 70.f;

	float CurrentFOV = 70.f;

	bool bIsShotToggleTrigger = false;

	UPROPERTY(EditDefaultsOnly, Category = "ASTRPGCharacter", Meta = (AllowPrivateAccess))
	float FirePerMinute = 600;

	FTimerHandle BetweenShotsTimer;

	float TimeBetweenFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> RifleFireAnimMontage;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	float CurrentAimPitch = 0.f;

	float PreviousAimPitch = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	float CurrentAimYaw = 0.f;

	float PreviousAimYaw = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess))
	TSubclassOf<class UCameraShakeBase> FireShake;

	FTimerHandle HittedRagdollRestoreTimer;
	FTimerDelegate HittedRagdollRestoreTimeDelegate;

	float TargetRagdollBlendWeight = 0.f;
	float CurrentRagdollBlendWeight = 0.f;
	bool bIsNowRagdollBlending = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AActor> LandMineClass;



};
