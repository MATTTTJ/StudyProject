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
    // 입력 로직 > 게임 로직 > 애니메이션 로직
    // 폰 오너가 죽었을 경우를 대비해 Try라는 함수를 사용한다. 
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
