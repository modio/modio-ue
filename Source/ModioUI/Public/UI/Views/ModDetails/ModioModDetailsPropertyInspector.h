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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/CommonComponents/ModioModPropertyInspectorBase.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioModDetailsPropertyInspector.generated.h"

/**
* Base class that stores details of a property inspector, which can retrieve
* arrays for style name and values
**/
UCLASS()
class MODIOUI_API UModioModDetailsPropertyInspector : public UModioModPropertyInspectorBase
{
	GENERATED_BODY()
protected:
	FText GetPropertyText(EModioModInfoPropertyType PropertyType);

	TArray<FString> GetStyleNames(const FModioUIStyleRef& StyleRef) const;

	UFUNCTION()
	TArray<FString> GetPropertyNameStyleNames() const;

	UFUNCTION()
	TArray<FString> GetPropertyValueStyleNames() const;

	UFUNCTION()
	const FModioRichTextStyle& GetTextStyle(FModioUIStyleRef TextStyle);

	virtual void NativeOnUpdatePropertyDisplay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioGridPanel* PropertyGrid;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	FText LastUpdatedFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance|Property Name Style",
			  meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef PropertyNameTextStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance|Property Name Style",
			  meta = (GetOptions = "GetPropertyNameStyleNames"))
	FName DefaultPropertyNameStyleName = FName("default");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance|Property Value Style",
			  meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef PropertyValueTextStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance|Property Value Style",
			  meta = (GetOptions = "GetPropertyValueStyleNames"))
	FName DefaultPropertyValueStyleName = FName("default");
};
