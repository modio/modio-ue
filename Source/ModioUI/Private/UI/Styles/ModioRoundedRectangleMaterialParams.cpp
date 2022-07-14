#include "UI/Styles/ModioRoundedRectangleMaterialParams.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"

UMaterialInterface* UModioRoundedRectangleMaterialParams::GetMaterialInstance()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			UMaterialInterface* NamedMaterial =
				DefaultStyleSet->GetNamedMaterial(FName("RoundedRectangle"), GetFName().ToString());
			if (UMaterialInstanceDynamic* NewInstance = Cast<UMaterialInstanceDynamic>(NamedMaterial))
			{
				NewInstance->SetScalarParameterValue(FName("RelativeRadius"), bRelativeRadius ? 1 : 0);
				NewInstance->SetScalarParameterValue(FName("EnableBorder"), bEnableBorder ? 1 : 0);
				NewInstance->SetScalarParameterValue(FName("BorderOpacity"), 1);
				NewInstance->SetScalarParameterValue(FName("Thickness"), BorderThickness);
				NewInstance->SetScalarParameterValue(FName("Radius"), CornerRadius);
				NewInstance->SetVectorParameterValue(FName("Color"),
													 NormalBorderColor.ResolveReference().GetSpecifiedColor());
				NewInstance->SetVectorParameterValue(FName("HoverColor"),
													 FocusedBorderColor.ResolveReference().GetSpecifiedColor());
				NewInstance->SetScalarParameterValue(FName("Hovered"), 0);
				NewInstance->SetVectorParameterValue(FName("ActiveColor"),
													 FocusedBorderColor.ResolveReference().GetSpecifiedColor());
				NewInstance->SetScalarParameterValue(FName("UseRenderTarget"), 0);
				NewInstance->SetVectorParameterValue(FName("InnerColor"),
													 InnerColor.ResolveReference().GetSpecifiedColor());
				NewInstance->SetScalarParameterValue(FName("EnableButtonGradients"), bEnableButtonGradients? 1 : 0);
				NewInstance->SetScalarParameterValue(FName("BorderGradientOpacity"), BorderGradientOpacity);
				NewInstance->SetScalarParameterValue(FName("InnerGradientOpacity"), InnerGradientOpacity);
				NewInstance->SetVectorParameterValue(FName("BorderGradientColor"),
													 BorderGradientColor.ResolveReference().GetSpecifiedColor());
				NewInstance->SetVectorParameterValue(FName("InnerGradientColor"),
													 InnerGradientColor.ResolveReference().GetSpecifiedColor());


				return NewInstance;
			}
		}
	}
	return nullptr;
}
#if WITH_EDITOR
void UModioRoundedRectangleMaterialParams::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) 
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GetMaterialInstance();
}
#endif
