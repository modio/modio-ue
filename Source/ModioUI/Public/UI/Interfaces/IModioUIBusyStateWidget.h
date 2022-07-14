#pragma once
#include "UObject/Interface.h"

#include "IModioUIBusyStateWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIBusyStateWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIBusyStateWidget : public IInterface
{
	GENERATED_BODY()

protected:
    virtual void NativeDisplayBusyState() {};

	void DisplayBusyState_Implementation()
    {
        NativeDisplayBusyState();
    }

    virtual void NativeEndBusyState(bool bErrorOccurred) 
    {
    };

	void EndBusyState_Implementation(bool bErrorOccurred)
    {
        NativeEndBusyState(bErrorOccurred);
    }

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisplayBusyState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndBusyState(bool bErrorOccurred);

};