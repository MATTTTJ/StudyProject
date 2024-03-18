// SW_Bar.cpp


#include "UI/SW_Bar.h"
#include "Components/ProgressBar.h"
#include "Component/SStatComponent.h"
#include "Characters/SNonPlayerCharacter.h"


USW_Bar::USW_Bar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) // 반드시 필요한 코드. 위젯 구현 할 때 꼭 넣어줘야 한다.
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
	// 하드 코딩으로 위젯과 속성을 반인드 하는 방법. 추후에 쉽게 바인딩 하는 방법으로 교체함

	check(nullptr != Bar);
}
