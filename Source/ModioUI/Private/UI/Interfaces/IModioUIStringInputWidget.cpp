#include "UI/Interfaces/IModioUIStringInputWidget.h"

void IModioUIStringInputWidget::SetHint_Implementation(const FText& InHintText) 
{
    NativeSetHintText(InHintText);
}

FString IModioUIStringInputWidget::GatherInput_Implementation()
{
    return NativeGatherInput();
}

void IModioUIStringInputWidget::SetInput_Implementation(const FString& Input)
{
	NativeSetInput(Input);
}
