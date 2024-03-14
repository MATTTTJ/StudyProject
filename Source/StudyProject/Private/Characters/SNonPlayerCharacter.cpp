// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SNonPlayerCharacter.h"
#include "Controllers/SAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animations/SAnimInstance.h"
#include "Characters/SRPGCharacter.h"
#include "Components/CapsuleComponent.h"

ASNonPlayerCharacter::ASNonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	
	// ASNonPlayerCharacter는 레벨에 배치되거나 새롭게 생성되면 SAIController의 빙의가 자동으로 진행됨.
}

void ASNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if(true == ::IsValid(AnimInstance) && false == AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::OnAttackAnimMontageEnded))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackAnimMontageEnded);
	}
}

float ASNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP = FMath::Clamp(CurrentHP - FinalDamageAmount, 0.f, MaxHP);

	if(CurrentHP < KINDA_SMALL_NUMBER)
	{
		ASRPGCharacter* DamageCauserCharacter = Cast<ASRPGCharacter>(DamageCauser);

		if(true == ::IsValid(DamageCauserCharacter))
		{
			DamageCauserCharacter->SetCurrentEXP(DamageCauserCharacter->GetCurrentEXP() + 5.f);
		}

		CurrentHP = 0.f;

		bIsDead = true;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		ASAIController* AIController = Cast<ASAIController>(GetController());

		if(true == ::IsValid(AIController))
		{
			AIController->EndAI();
		}
	}


	return FinalDamageAmount;
}

void ASNonPlayerCharacter::Attack()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + AttackRange * GetActorForwardVector(),
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if(true == bResult)
	{
		if(true == ::IsValid(HitResult.GetActor()))
		{
			//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[NPC] Hit Actor Name : %s"), *HitResult.GetActor()->GetName()));

			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[NPC] Hit Actor Class Name : %s"), *HitResult.GetActor()->GetClass()->GetName()));
		}
	}

	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if(true == ::IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackAnimMontage();
		bIsAttacking = true;
		if(false == AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::OnAttackAnimMontageEnded))
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackAnimMontageEnded);
		}
	}

#pragma region CollisionDebugDrawing
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = true == bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#pragma endregion
}

void ASNonPlayerCharacter::OnAttackAnimMontageEnded(UAnimMontage* Montage, bool bIsInterrupt)
{
	bIsAttacking = false;
}
