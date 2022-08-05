#pragma once
#include "UI/Commands/ModioUIMenuCommandList.h"
#include "UObject/Interface.h"

#include "IModioUIPopupMenuContentWidget.generated.h"

DECLARE_DELEGATE(FOnContentClose);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIPopupMenuContentWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIPopupMenuContentWidget : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeSetMenuEntries(const struct FModioUIMenuCommandList& Entries) {}
	virtual void NativeSetDesiredSize(const FVector2D& DesiredSize) {}

	virtual FOnContentClose& NativeGetContentCloseDelegate()
	{
		static FOnContentClose OnContentCloseInstance;
		return OnContentCloseInstance;
	}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIPopupMenuContentWidget")
	void SetMenuEntries(const FModioUIMenuCommandList& Entries);

	void SetMenuEntries_Implementation(const FModioUIMenuCommandList& Entries);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIPopupMenuContentWidget")
	void SetDesiredSize(const FVector2D& DesiredSize);

	void SetDesiredSize_Implementation(const FVector2D& DesiredSize);

	FOnContentClose& GetContentCloseDelegate();

	// NativeSetContentPadding?
	// NativeSetTableRowStyle?
	// NativeSetTextAlignment?
	// Maybe collapse all these into a setstyle function that sets the popup style entirely?
};