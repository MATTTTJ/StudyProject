// SAnimInstance.cpp


#include "Animations/SAnimInstance.h"
#include "Characters/SRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USAnimInstance::USAnimInstance()
{
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    // �Է� ���� > ���� ���� > �ִϸ��̼� ����
    // �� ���ʰ� �׾��� ��츦 ����� Try��� �Լ��� ����Ѵ�. 
    ASRPGCharacter* OwnerCharacter = Cast<ASRPGCharacter>(TryGetPawnOwner());
    if (true == ::IsValid(OwnerCharacter))
    {
        UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
        if (true == ::IsValid(CharacterMovementComponent))
        {
            Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
            CurrentSpeed = Velocity.Size();
            bIsFalling = CharacterMovementComponent->IsFalling();
            bIsCrouching = CharacterMovementComponent->IsCrouching();
        }
    }
}

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

    CurrentSpeed = 0.f;

    Velocity = FVector::ZeroVector;

    bIsFalling = false;

    bIsCrouching = false;
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