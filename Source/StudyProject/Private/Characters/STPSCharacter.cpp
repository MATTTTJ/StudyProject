// STPSCharacter.cpp


#include "Characters/STPSCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/SStatComponent.h"
#include "Inputs/SInputConfigData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "WorldStatics/SLandMine.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ASTPSCharacter::ASTPSCharacter()
	: ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SCharacter"));

	CameraComponent->SetRelativeLocation(FVector(0.f, 60.f, 20.f)); // TPS 방식의 슈팅 게임 특징

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	// 반대가 되어야하는거 아닌가?
	TimeBetweenFire = 60.f / FirePerMinute;

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshComponent"));
}

void ASTPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(true == ::IsValid(GetController()))
	{
		PreviousAimPitch = CurrentAimPitch;
		PreviousAimYaw = CurrentAimYaw;

		FRotator ControlRotation = GetController()->GetControlRotation();
		CurrentAimPitch = ControlRotation.Pitch;
		CurrentAimYaw = ControlRotation.Yaw;

		if(PreviousAimPitch != CurrentAimPitch || PreviousAimYaw != CurrentAimYaw)
		{
			if(false == HasAuthority()) // 서버에선 원래 Update 되지 않았으므로, 굳이 호출할 필요 없다.
			{
				UpdateAimValue_Server(CurrentAimPitch, CurrentAimYaw);
			}
		}
	}

	if(PreviousForwardInputValue != ForwardInputValue || PreviousRightInputValue != RightInputValue)
	{
		if (false == HasAuthority()) // 서버에선 원래 Update 되지 않았으므로, 굳이 호출할 필요 없다.
		{
			UpdateInputValue_Server(ForwardInputValue, RightInputValue);
		}
	}

	if(true == bIsNowRagdollBlending)
	{
		CurrentRagdollBlendWeight = FMath::FInterpTo(CurrentRagdollBlendWeight, TargetRagdollBlendWeight, DeltaSeconds, 10.f);
		FName PivotBoneName = FName(TEXT("spine_01"));
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PivotBoneName, CurrentRagdollBlendWeight);

		if(CurrentRagdollBlendWeight - TargetRagdollBlendWeight < KINDA_SMALL_NUMBER)
		{
			GetMesh()->SetAllBodiesBelowSimulatePhysics(PivotBoneName, false);
			bIsNowRagdollBlending = false;
		}

		if(true == ::IsValid(GetStatComponent()) && GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
		{
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("root")), 1.f);
			// 모든 본에 가중치
			GetMesh()->SetSimulatePhysics(true);
			bIsNowRagdollBlending = false;
		}
	}


	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);



}

void ASTPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(true == ::IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(true == ::IsValid(Subsystem))
		{
			Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
		}
	}

	FName EquipmentSocketName = FName(TEXT("EquipmentSocket"));
	if(true == GetMesh()->DoesSocketExist(EquipmentSocketName))
	{
		WeaponSkeletalMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipmentSocketName);
	}
}

float ASTPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(false == ::IsValid(GetStatComponent()))
	{
		return ActualDamage;
	}

	if(GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		FName PivotBoneName = FName(TEXT("spine_01"));
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PivotBoneName, true); // 상체만 랙돌 적용
		TargetRagdollBlendWeight = 0.1f;
		HittedRagdollRestoreTimeDelegate.BindUObject(this, &ThisClass::OnHittedRagdollRestoreTimerElapsed);
		GetWorld()->GetTimerManager().SetTimer(HittedRagdollRestoreTimer, HittedRagdollRestoreTimeDelegate, 1.f, false);
	}

	return ActualDamage;
}

void ASTPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ForwardInputValue);
	DOREPLIFETIME(ThisClass, RightInputValue);
	DOREPLIFETIME(ThisClass, CurrentAimPitch);
	DOREPLIFETIME(ThisClass, CurrentAimYaw);
}

void ASTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(true == ::IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->IronSightAction, ETriggerEvent::Started, this, &ThisClass::StartIronSight);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->IronSightAction, ETriggerEvent::Completed, this, &ThisClass::EndInronSight);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->ShotTriggerAction, ETriggerEvent::Started, this, &ThisClass::ToggleTrigger);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LandMineAction, ETriggerEvent::Started, this, &ThisClass::SpawnLandMine);
	}
}

void ASTPSCharacter::Move(const FInputActionValue& InValue)
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

void ASTPSCharacter::Look(const FInputActionValue& InValue)
{
	FVector2D LookAxisVector = InValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASTPSCharacter::Attack(const FInputActionValue& InValue)
{
	// Fire 하나로 해도 되지 않을까 싶은
	if(false == bIsShotToggleTrigger)
	{
		Fire();
	}
}

void ASTPSCharacter::Fire()
{
	if (true == HasAuthority() || GetOwner() != UGameplayStatics::GetPlayerController(this, 0))
		return;

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASTPSCharacter::Attack() has been called.")));
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if(false == ::IsValid(PlayerController))
	{
		return;
	}

	FHitResult HitResult;
	FVector CameraStartLocation = CameraComponent->GetComponentLocation();
	FVector CameraEndLocation = CameraStartLocation + CameraComponent->GetForwardVector() * 5000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredComponent((const UPrimitiveComponent*)CameraComponent);
	QueryParams.bTraceComplex = true;
	/* 충돌감지 할 때 좀 더 복잡한 모양의 충돌체를 기준으로 검사할지에 대한 속성
		Content Browser > StarterContent > props > SM_Chair 더블클릭
		Details > Collision Complexity를 Use Complex Collision As Simple 로 설정하면
		쿼리(충돌 감지) 요청시 복잡한 모양에 대한 쿼리를 제공한다.
		충돌 계산 부하는 증가하지만 그만큼 현실적인 게임 플레이가 가능하다.
		SM_Chair > Toolbar > Collision > Auto Convex Collision 클릭 후 우하단 Convex Decomposition 으로
		복잡한 모양의 충돌체를 쉽게 제작 가능하다.
	*/

	FVector MuzzleLocation = WeaponSkeletalMeshComponent->GetSocketLocation(FName("MuzzleSocket"));
	bool bIsCollide = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, CameraEndLocation, ECC_GameTraceChannel2, QueryParams);
	/*DrawDebugLine(GetWorld(), CameraStartLocation, CameraEndLocation, FColor(255, 0, 0, 255), false, 20.f, 0U, 5.f);
	DrawDebugSphere(GetWorld(), CameraStartLocation, 3.f, 16, FColor(0, 255, 0, 255), false, 20.f, 0U, 5.f);
	DrawDebugSphere(GetWorld(), CameraEndLocation, 3.f, 16, FColor(0, 0, 255, 255), false, 20.f, 0U, 5.f);*/

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(false == ::IsValid(AnimInstance))
	{
		return;
	}

	if(true == bIsCollide)
	{
		//DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.Location, FColor(255, 255, 255, 64), true, 0.1f, 0u, 0.5f);

		ASCharacter* HittedCharacter = Cast<ASCharacter>(HitResult.GetActor());
		if(true == ::IsValid(HittedCharacter))
		{
			FDamageEvent DamageEvent;

			FString BoneNameString = HitResult.BoneName.ToString();

			if(true == BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
			{
				ApplyDamageAndDrawLine_Server(MuzzleLocation, HitResult.Location, HittedCharacter, 100.f, DamageEvent, GetController(), this);

				//HittedCharacter->TakeDamage(100.f, DamageEvent, GetController(), this);
			}
			else
			{
				ApplyDamageAndDrawLine_Server(MuzzleLocation, HitResult.Location, HittedCharacter, 10.f, DamageEvent, GetController(), this);
				//HittedCharacter->TakeDamage(10.f, DamageEvent, GetController(), this);
			}
		}
	}
	else
	{
		//DrawDebugLine(GetWorld(), MuzzleLocation, CameraEndLocation, FColor(255, 255, 255, 64), true, 0.1f, 0u, 0.5f);
		FDamageEvent DamageEvent;
		ApplyDamageAndDrawLine_Server(MuzzleLocation, CameraEndLocation, nullptr, 0.f, DamageEvent, GetController(), this);
	}

	if(false == AnimInstance->Montage_IsPlaying(RifleFireAnimMontage))
	{
		AnimInstance->Montage_Play(RifleFireAnimMontage);
		PlayAttackMontage_Server();
	}

	if (true == ::IsValid(FireShake))
	{
		if(GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			// 다른 클라가 사격했는데, 내 PC화면이 흔들리지 않게끔
			PlayerController->ClientStartCameraShake(FireShake);
		}

	}
}

void ASTPSCharacter::ToggleTrigger(const FInputActionValue& InValue)
{
	bIsShotToggleTrigger = !bIsShotToggleTrigger;

	if(true == bIsShotToggleTrigger)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Now Multi Shot Mode")));
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Now Single Shot Mode")));
	}

}

void ASTPSCharacter::StartFire(const FInputActionValue& InValue)
{
	if(true == bIsShotToggleTrigger)
	{
		GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::Fire, TimeBetweenFire, true);
	}
}

void ASTPSCharacter::StopFire(const FInputActionValue& InValue)
{
	GetWorldTimerManager().ClearTimer(BetweenShotsTimer);
}

void ASTPSCharacter::StartIronSight(const FInputActionValue& InValue)
{
	TargetFOV = 45.f;
}

void ASTPSCharacter::EndInronSight(const FInputActionValue& InValue)
{
	TargetFOV = 70.f;
}

void ASTPSCharacter::SpawnLandMine(const FInputActionValue& InValue)
{
	SpawnLandMine_Server();
}

void ASTPSCharacter::SpawnLandMine_Server_Implementation()
{
	if(true == ::IsValid(LandMineClass))
	{
		FVector SpawnedLocation = (GetActorLocation() + GetActorForwardVector() * 200.f) - FVector(0.f, 0.f, 90.f);
		ASLandMine* SpawnedLandMine = GetWorld()->SpawnActor<ASLandMine>(LandMineClass, SpawnedLocation, FRotator::ZeroRotator);
		SpawnedLandMine->SetOwner(GetController());
	}
}

bool ASTPSCharacter::SpawnLandMine_Server_Validate()
{
	// 검증용 함수, 아마도 스킬 쿨타임 체크 또는 사용 횟수 이중 체크하면 좋지않을까?

	return true;
}

void ASTPSCharacter::OnHittedRagdollRestoreTimerElapsed()
{
	TargetRagdollBlendWeight = 0.f;
	CurrentRagdollBlendWeight = 1.f;
	bIsNowRagdollBlending = true;
}

void ASTPSCharacter::ApplyDamageAndDrawLine_Server_Implementation(const FVector& InDrawStart, const FVector& InDrawEnd,
	ACharacter* InHittedCharacter, float InDamage, FDamageEvent const& InDamageEvent, AController* InEventInstigator,
	AActor* InDamageCauser)
{
	if(true == ::IsValid(InHittedCharacter))
	{
		InHittedCharacter->TakeDamage(InDamage, InDamageEvent, InEventInstigator, InDamageCauser);
	}
	DrawLine_NetMulticast(InDrawStart, InDrawEnd);
}

void ASTPSCharacter::DrawLine_NetMulticast_Implementation(const FVector& InDrawStart, const FVector& InDrawEnd)
{
	DrawDebugLine(GetWorld(), InDrawStart, InDrawEnd, FColor(255, 255, 255, 64), false, 0.1f, 0U, 0.5f);
}

void ASTPSCharacter::PlayAttackMontage_Server_Implementation()
{
	PlayAttackMontage_NetMulticast();
}

void ASTPSCharacter::PlayAttackMontage_NetMulticast_Implementation()
{
	if (false == HasAuthority() && GetOwner() != UGameplayStatics::GetPlayerController(this, 0))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if(false == ::IsValid(AnimInstance))
		{
			return;
		}

		if(false == AnimInstance->Montage_IsPlaying(RifleFireAnimMontage))
		{
			AnimInstance->Montage_Play(RifleFireAnimMontage);
		}
	}
}

void ASTPSCharacter::UpdateInputValue_Server_Implementation(const float& InForwardInputValue,
                                                            const float& InRightInputValue)
{
	ForwardInputValue = InForwardInputValue;
	RightInputValue = InRightInputValue;
}

void ASTPSCharacter::UpdateAimValue_Server_Implementation(const float& InAimPitchValue, const float& InAimYawValue)
{
	CurrentAimPitch = InAimPitchValue;
	CurrentAimYaw = InAimYawValue;
}
