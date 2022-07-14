#include "UI/Styles/ModioWidgetBorderStyle.h"

FName const FModioWidgetBorderStyle::TypeName = FName("FModioWidgetBorderStyle");

void ApplyModioBorderStyle(UMaterialInstanceDynamic* WidgetMaterial, const FModioWidgetBorderStyle* NewStyle)
{
	if (NewStyle && NewStyle->bMaskWithMaterial)
	{
		WidgetMaterial->SetScalarParameterValue(FName("RelativeRadius"), NewStyle->bRelativeRadius ? 1 : 0);
		WidgetMaterial->SetScalarParameterValue(FName("EnableBorder"), NewStyle->bEnableBorder ? 1 : 0);
		WidgetMaterial->SetScalarParameterValue(FName("BorderOpacity"), NewStyle->BorderOpacity);
		WidgetMaterial->SetScalarParameterValue(FName("Thickness"), NewStyle->BorderThickness);
		WidgetMaterial->SetScalarParameterValue(FName("Radius"), NewStyle->CornerRadius);
		WidgetMaterial->SetVectorParameterValue(FName("Color"),
												NewStyle->BorderColor.ResolveReference().GetSpecifiedColor());
		WidgetMaterial->SetVectorParameterValue(FName("HoverColor"),
												NewStyle->HoverColor.ResolveReference().GetSpecifiedColor());
		WidgetMaterial->SetScalarParameterValue(FName("Hovered"), 0);
		WidgetMaterial->SetVectorParameterValue(FName("ActiveColor"),
												NewStyle->ActiveColor.ResolveReference().GetSpecifiedColor());
		WidgetMaterial->SetScalarParameterValue(FName("UseRenderTarget"), NewStyle->bUseWidgetTexture ? 1 : 0);
		WidgetMaterial->SetVectorParameterValue(FName("InnerColor"), NewStyle->InnerColor);

		// Default values for these parameters as they aren't currently exposed on the WidgetBorderStyle but are present
		// in our material
		WidgetMaterial->SetScalarParameterValue(FName("EnableButtonGradients"), 0);
		WidgetMaterial->SetScalarParameterValue(FName("BorderGradientOpacity"), 0);
		WidgetMaterial->SetScalarParameterValue(FName("InnerGradientOpacity"), 0);
		WidgetMaterial->SetVectorParameterValue(FName("BorderGradientColor"), FLinearColor::Black);
		WidgetMaterial->SetVectorParameterValue(FName("InnerGradientColor"), FLinearColor::Black);
	}
}
