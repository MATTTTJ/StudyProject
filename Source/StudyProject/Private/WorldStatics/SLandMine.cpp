// SLandMine.cpp

#include "WorldStatics/SLandMine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

ASLandMine::ASLandMine()
	: bIsExploded(false)
{
	PrimaryActorTick.bCanEverTick = true;

	BodyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyBoxComponent"));
	SetRootComponent(BodyBoxComponent);

	BodyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyStaticMeshComponent"));
	BodyStaticMeshComponent->SetupAttachment(GetRootComponent());

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->SetAutoActivate(false);
}

void ASLandMine::BeginPlay()
{
	Super::BeginPlay();

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called.")));

	// 서버에서 실행중인지? 
	if(true == HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called in Server PC.")));
	}
	else
	{
		if(GetOwner() == UGameplayStatics::GetPlayerController(this,0))
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called in OwningClient PC.")));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called in OtherClient PC.")));
		}
	}

	if(false == OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ASLandMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called.")));
	if (true == HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called in Server PC.")));
	}
	else
	{
		if (GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called in OwningClient PC.")));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called in OtherClient PC.")));
		}
	}

	if(true == OnActorBeginOverlap.IsAlreadyBound(this, & ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.RemoveDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ASLandMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASLandMine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsExploded);
}

void ASLandMine::OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (true == HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::OnLandMineBeginOverlap() has been called in Server PC.")));
	}
	else
	{
		if (GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::OnLandMineBeginOverlap() has been called in OwningClient PC.")));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::OnLandMineBeginOverlap() has been called in OtherClient PC.")));
		}
	}
	
	if (true == HasAuthority() && false == bIsExploded)
	{
		SpawnEffect_NetMulticast();
		bIsExploded = true;
	}
}

void ASLandMine::OnRep_IsExploded()
{
	if(true == bIsExploded)
	{
		BodyStaticMeshComponent->SetMaterial(0, ExplodedMaterial);
	}
}

void ASLandMine::SpawnEffect_NetMulticast_Implementation()
{
	ParticleSystemComponent->SetActive(true);

	if(true == ::IsValid(ExplodedMaterial))
	{
		BodyStaticMeshComponent->SetMaterial(0, ExplodedMaterial);
	}
}

