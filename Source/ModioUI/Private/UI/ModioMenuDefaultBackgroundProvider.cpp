#include "UI/ModioMenuDefaultBackgroundProvider.h"


UMaterialInterface* UModioMenuDefaultBackgroundProvider::NativeGetBackgroundMaterial()
{
	FSoftObjectPath Fallback("/Modio/UI/Materials/M_UI_Background_Waves");
	BackgroundMaterial = Cast<UMaterialInterface>(Fallback.TryLoad());
	return BackgroundMaterial;
}

