// SW_Bar.cpp


#include "UI/SW_Bar.h"
#include "Components/ProgressBar.h"
#include "Component/SStatComponent.h"
#include "Characters/SNonPlayerCharacter.h"


USW_Bar::USW_Bar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) // �ݵ�� �ʿ��� �ڵ�. ���� ���� �� �� �� �־���� �Ѵ�.
{
}

void USW_Bar::SetMaxFigure(float InMaxFigure)
{
	if(InMaxFigure < KINDA_SMALL_NUMBER)
	{
		MaxFigure = 0.f;
		return;
	}

	MaxFigure = InMaxFigure;
}

void USW_Bar::NativeConstruct()
{
	Super::NativeConstruct();

	Bar = Cast<UProgressBar>(GetWidgetFromName("BarWidget"));
	// �ϵ� �ڵ����� ������ �Ӽ��� ���ε� �ϴ� ���. ���Ŀ� ���� ���ε� �ϴ� ������� ��ü��

	check(nullptr != Bar);
}