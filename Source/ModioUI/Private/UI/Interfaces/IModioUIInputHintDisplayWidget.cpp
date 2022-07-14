#include "UI/Interfaces/IModioUIInputHintDisplayWidget.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIInputHintDisplayWidget::DisplayHintForInputHandler(FKey VirtualKey)
{
	bRoutedDisplayHintForInput = false;
	NativeDisplayHintForInput(VirtualKey);
	checkf(bRoutedDisplayHintForInput,
		   TEXT("Please call IModioUIDisplayHintForInputReceiver::NativeDisplayHintForInput in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIInputHintDisplayWidget::NativeDisplayHintForInput(FKey VirtualKey)
{
	bRoutedDisplayHintForInput = true;
	Execute_OnDisplayHintForInput(Cast<UObject>(this), VirtualKey);
}