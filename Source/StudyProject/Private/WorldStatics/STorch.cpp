// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldStatics/STorch.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


ASTorch::ASTorch()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BodyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyStaticMeshComponent"));
	BodyStaticMeshComponent->SetupAttachment(GetRootComponent());
	BodyStaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -30.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500'"));

	if(true == BodyStaticMesh.Succeeded())
	{
		BodyStaticMeshComponent->SetStaticMesh(BodyStaticMesh.Object);
	}

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(GetRootComponent());
	PointLightComponent->SetRelativeLocation(FVector(0.f, 0.f, 500.f));

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->SetRelativeLocation(FVector(0.f, 0.f, 500.f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleTemplate(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	if(true == ParticleTemplate.Succeeded())
	{
		ParticleSystemComponent->SetTemplate(ParticleTemplate.Object);
	}

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
}

void ASTorch::BeginPlay()
{
	Super::BeginPlay();

	RotationSpeed = 300.f;
	RotatingMovementComponent->RotationRate = FRotator(0.f, RotationSpeed, 0.f);
}

void ASTorch::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//AddActorWorldRotation(FRotator(0.f, RotationSpeed * DeltaSeconds, 0.f));
}