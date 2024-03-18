// SRPGCharacter.cpp

#include "Characters/SRPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animations/SAnimInstance.h"
#include "Inputs/SInputConfigData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Game/SPlayerState.h"
#include "Particles/ParticleSystemComponent.h"

ASRPGCharacter::ASRPGCharacter()
	: bIsAttacking(false)
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->bDoCollisionTest = true;
    SpringArmComponent->bInheritPitch = true;
    SpringArmComponent->bInheritYaw = true;
    SpringArmComponent->bInheritRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

    GetCapsuleComponent()->SetCollisionProfileName(TEXT("SCharacter"));

    // EXP
    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(GetRootComponent());
    ParticleSystemComponent->SetAutoActivate(false);
}

void ASRPGCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (true == ::IsValid(PlayerController))
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (true == ::IsValid(Subsystem))
        {
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }

    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    if(true == ::IsValid(AnimInstance))
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackMontageEnded);
        AnimInstance->OnCheckHitDelegate.AddDynamic(this, &ThisClass::CheckHit);
        AnimInstance->OnCheckCanNextComboDelegate.AddDynamic(this, &ThisClass::CheckCanNextCombo);
    }

    ASPlayerState* PS = GetPlayerState<ASPlayerState>();
    if(true == ::IsValid(PS))
    {
	    if(false == PS->OnCurrentLevelChangedDelegate.IsAlreadyBound(this, &ThisClass::OnCurrentLevelChanged))
	    {
            PS->OnCurrentLevelChangedDelegate.AddDynamic(this, &ThisClass::OnCurrentLevelChanged);
	    }
    }
}

void ASRPGCharacter::OnCurrentLevelChanged(int32 InOldCurrentLevel, int32 InNewCurrentLevel)
{
    ParticleSystemComponent->Activate(true);
}

void ASRPGCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    bIsAttacking = false;
}

float ASRPGCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
    float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

  
    return FinalDamageAmount;
}

void ASRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (true == ::IsValid(EnhancedInputComponent))
    {
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
    }
}

void ASRPGCharacter::Move(const FInputActionValue& InValue)
{
    FVector2D MovementVector = InValue.Get<FVector2D>();
    ForwardInputValue = MovementVector.X;
    RightInputValue = MovementVector.Y;

    const FRotator CurrentControlRotation = GetController()->GetControlRotation();
    const FRotator CurrentControlRotationYaw(0.f, CurrentControlRotation.Yaw, 0.f);

    FVector ForwardDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::X);
    FVector RightDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementVector.X);
    AddMovementInput(RightDirection, MovementVector.Y);
}

void ASRPGCharacter::Look(const FInputActionValue& InValue)
{
    FVector2D LookAxisVector = InValue.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ASRPGCharacter::Attack(const FInputActionValue& InValue)
{
    // 체공중 공격불가
    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    if(false == ::IsValid(AnimInstance))
    {
        return;
    }

    if (true == AnimInstance->bIsFalling)
        return;
    // ~체공중 공격불가

    if(0 == CurrentComboCount)
    {
        BeginCombo();
        return;
    }
    else
    {
        ensure(FMath::IsWithinInclusive<int32>(CurrentComboCount, 1, MaxComboCount));
        bIsAttackKeyPressed = true;
    }

    //UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Attack() has been called")));

   /* USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    if(true == ::IsValid(AnimInstance))
    {
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
        AnimInstance->PlayAttackAnimMontage();
        bIsAttacking = true;
    }*/
}

void ASRPGCharacter::CheckHit()
{
    //UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("CheckHit() has been called")));

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
            //UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName()));

            FDamageEvent DamageEvent;
            HitResult.GetActor()->TakeDamage(50.f, DamageEvent, GetController(), this);
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

void ASRPGCharacter::BeginCombo()
{
    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    if(false == ::IsValid(AnimInstance))
    {
        return;
    }

    CurrentComboCount = 1;

    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    AnimInstance->PlayAttackAnimMontage();

    FOnMontageEnded OnMontageEndedDelegate;
    OnMontageEndedDelegate.BindUObject(this, &ThisClass::EndCombo);
    AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, AnimInstance->AttackAnimMontage);
}

void ASRPGCharacter::CheckCanNextCombo()
{
    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance))
    {
        return;
    }

    if(true == bIsAttackKeyPressed)
    {
        CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);

        FName NextSectionName = *FString::Printf(TEXT("%s%d"), *AttackAnimMontageSectionName, CurrentComboCount);
        AnimInstance->Montage_JumpToSection(NextSectionName, AnimInstance->AttackAnimMontage);
        bIsAttackKeyPressed = false;
    }
}

void ASRPGCharacter::EndCombo(UAnimMontage* InAnimMontage, bool bInterrupted)
{
    ensure(0 != CurrentComboCount);
    CurrentComboCount = 0;
    bIsAttackKeyPressed = false;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
