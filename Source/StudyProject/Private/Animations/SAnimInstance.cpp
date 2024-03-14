// SAnimInstance.cpp


#include "Animations/SAnimInstance.h"
#include "Characters/SNonPlayerCharacter.h"
#include "Characters/SRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/SStatComponent.h"

USAnimInstance::USAnimInstance()
{
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    // �Է� ���� > ���� ���� > �ִϸ��̼� ����
    // �� ���ʰ� �׾��� ��츦 ����� Try��� �Լ��� ����Ѵ�. 
    ASCharacter* OwnerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
    if (true == ::IsValid(OwnerCharacter))
    {
        UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
        if (true == ::IsValid(CharacterMovementComponent))
        {
            Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
            CurrentSpeed = Velocity.Size();
            bIsFalling = CharacterMovementComponent->IsFalling();
            bIsCrouching = CharacterMovementComponent->IsCrouching();
            //bIsDead = OwnerCharacter->IsDead();
        }
    }

    // ���÷��̾� ���ǵ� üũ �α�
   /* ASNonPlayerCharacter* NonPlayer = Cast<ASNonPlayerCharacter>(TryGetPawnOwner());
    if(true == ::IsValid(NonPlayer))
    {
        if(CurrentSpeed > KINDA_SMALL_NUMBER)
			UE_LOG(LogTemp, Error, TEXT("Current Speed [ %.3f ]"), CurrentSpeed);
    }*/
}

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

    CurrentSpeed = 0.f;

    Velocity = FVector::ZeroVector;

    bIsFalling = false;

    bIsCrouching = false;

    bIsDead = false;

    ASCharacter* OwnerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
    if(true ==::IsValid(OwnerCharacter))
    {
        USStatComponent* StatComponent = OwnerCharacter->GetStatComponent();
        if(true == ::IsValid(StatComponent))
        {
	        if(false == StatComponent->OnOutOfCurrentHPDelegate.IsAlreadyBound(this, &ThisClass::OnCharacterDeath))
	        {
                StatComponent->OnOutOfCurrentHPDelegate.AddDynamic(this, &ThisClass::OnCharacterDeath);
	        }
        }
    }
}

void USAnimInstance::PlayAttackAnimMontage()
{
    if(true == ::IsValid(AttackAnimMontage))
    {
	    if(false == Montage_IsPlaying(AttackAnimMontage))
	    {
            Montage_Play(AttackAnimMontage);
	    }
    }
}

void USAnimInstance::AnimNotify_CheckHit()
{
    if(true == OnCheckHitDelegate.IsBound())
    {
        OnCheckHitDelegate.Broadcast();
    }
}

void USAnimInstance::AnimNotify_CheckCanNextCombo()
{
    if(true == OnCheckCanNextComboDelegate.IsBound())
    {
        OnCheckCanNextComboDelegate.Broadcast();
    }
}

void USAnimInstance::OnCharacterDeath()
{
    bIsDead = true;
}
