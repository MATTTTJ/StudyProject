// SAIController.cpp


#include "Controllers/SAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

const float ASAIController::PatrolRepeatInterval(3.f);
const float ASAIController::PatrolRadius(500.f);

ASAIController::ASAIController()
{
}

void ASAIController::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &ThisClass::OnPatrolTimerElapsed, PatrolRepeatInterval, true);
}

void ASAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
}

void ASAIController::OnPatrolTimerElapsed()
{
    APawn* ControlledPawn = GetPawn();
    if (true == ::IsValid(ControlledPawn))
    {
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
        if (true == ::IsValid(NavSystem))
        {
            FNavLocation NextLocation;
            if (true == NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, PatrolRadius, NextLocation))
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
            }
        }
    }
}