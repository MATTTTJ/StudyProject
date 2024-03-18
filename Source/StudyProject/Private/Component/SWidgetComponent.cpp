// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SWidgetComponent.h"

#include "UI/StudyUserWidget.h"

void USWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UStudyUserWidget* SWidget = Cast<UStudyUserWidget>(GetWidget());
	if(true == ::IsValid(SWidget))
	{
		SWidget->SetOwningActor(GetOwner());
	}
}
