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

#include "Components/EditableText.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioEditableText.generated.h"

/**
* Modio UI element that provides an editable textbox with custom styles
**/
UCLASS()
class MODIOUI_API UModioEditableText : public UEditableText
{
	GENERATED_BODY()
protected:
	virtual void SynchronizeProperties() override;

	/**
	* Retrieve the style name for this editable text
	* @return An array of strings with style names
	**/
	UFUNCTION()
	TArray<FString> GetStyleNames() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance", meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance", meta = (GetOptions = "GetStyleNames"))
	FName DefaultStyleName = FName("default");
};
