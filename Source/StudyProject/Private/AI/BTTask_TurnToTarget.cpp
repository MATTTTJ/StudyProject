// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SNonPlayerCharacter.h"
#include "Controllers/SAIController.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTargetActor");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);;

	ASAIController* AIC = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if(true == ::IsValid(AIC))
	{
		ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIC->GetPawn());
		if(true == ::IsValid(NPC))
		{
			if(ASCharacter* TargetPC = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIC->TargetActorKey)))
			{
				// 목표 지점 - 내 지점 = 목표를 바라보는 룩벡터
				FVector LookVector = TargetPC->GetActorLocation() - NPC->GetActorLocation();
				// 기울지 않도록 Z 값 0 고정
				LookVector.Z = 0.f;
				FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();
				NPC->SetActorRotation(FMath::RInterpTo(NPC->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.f));

				return Result = EBTNodeResult::Succeeded;
			}
		}
	}

	return Result = EBTNodeResult::Failed;
}
