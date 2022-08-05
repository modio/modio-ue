#pragma once
#include "Components/SlateWrapperTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "UObject/Interface.h"

#include "IModioUIClickableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FEventReply, FOnClickedEx, UWidget*, Widget);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIClickableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIClickableWidget : public IInterface
{
	GENERATED_BODY()
protected:
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIClickableWidget")
	void AddClickHandler(UObject* UserObject, FName FunctionName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIClickableWidget")
	void RemoveClickHandler(UObject* UserObject, FName FunctionName);
};