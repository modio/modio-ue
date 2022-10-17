/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "UI/Commands/ModioUIMenuCommandList.h"
#include "UObject/Interface.h"
#include "UI/Styles/ModioUIStyleRef.h"

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
	virtual void NativeSetStyle(const FModioUIStyleRef StyleRef) {}

	virtual FOnContentClose& NativeGetContentCloseDelegate()
	{
		static FOnContentClose OnContentCloseInstance;
		return OnContentCloseInstance;
	}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIPopupMenuContentWidget")
	void SetMenuEntries(const FModioUIMenuCommandList& Entries);

	void SetMenuEntries_Implementation(const FModioUIMenuCommandList& Entries);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIPopupMenuContentWidget")
	void SetStyle(const FModioUIStyleRef& StyleRef);

	void SetStyle_Implementation(FModioUIStyleRef StyleRef);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIPopupMenuContentWidget")
	void SetDesiredSize(const FVector2D& DesiredSize);

	void SetDesiredSize_Implementation(const FVector2D& DesiredSize);

	FOnContentClose& GetContentCloseDelegate();

	// NativeSetContentPadding?
	// NativeSetTableRowStyle?
	// NativeSetTextAlignment?
	// Maybe collapse all these into a setstyle function that sets the popup style entirely?
};
