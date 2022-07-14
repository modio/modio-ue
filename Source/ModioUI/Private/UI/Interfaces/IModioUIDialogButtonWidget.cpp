#include "UI/Interfaces/IModioUIDialogButtonWidget.h"

void IModioUIDialogButtonWidget::SetStyle_Implementation(const FModioDialogStyle& Style)
{
    NativeSetStyle(Style);
}

void IModioUIDialogButtonWidget::SetDialogController_Implementation(class UModioDialogController* Controller) 
{
	NativeSetDialogController(Controller);
}
