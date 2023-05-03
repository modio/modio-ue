#include "ModioWidgetStyleData.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

void UModioWidgetStyleData::LoadStyleBrushes() 
{
	for (auto& style : WidgetStyles)
	{
		UModioUIWidgetStyleContainer* container = Cast<UModioUIWidgetStyleContainer>(style.Value);
		if (IsValid(container))
		{
			container->LoadBrushMaterials();
		}
	}
}