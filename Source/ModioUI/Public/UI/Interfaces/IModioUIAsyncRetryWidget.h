#pragma once
#include "Delegates/Delegate.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UObject/Interface.h"

#include "IModioUIAsyncRetryWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnRetryRequested);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIAsyncRetryWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIAsyncRetryWidget : public IInterface
{
	GENERATED_BODY()

protected:
	FOnRetryRequested OnRetryRequestedDelegate;
	void SetRetryRequestedDelegate_Implementation(const FOnRetryRequested& Delegate)
	{
		OnRetryRequestedDelegate = Delegate;
	}

public:
	/// @brief Call this to pass in a delegate that will receive operation state change notifications
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIAsyncRetryWidget")
	void SetRetryRequestedDelegate(const FOnRetryRequested& Delegate);
};