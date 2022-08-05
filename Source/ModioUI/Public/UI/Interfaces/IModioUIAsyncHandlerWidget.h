#pragma once
#include "Delegates/Delegate.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UObject/Interface.h"

#include "IModioUIAsyncHandlerWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIAsyncHandlerWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIAsyncHandlerWidget : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeLinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget) {

	}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIAsyncHandlerWidgets")
	void LinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget);
	void LinkAsyncOperationWidget_Implementation(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget)
	{
		NativeLinkAsyncOperationWidget(Widget);
	}
};