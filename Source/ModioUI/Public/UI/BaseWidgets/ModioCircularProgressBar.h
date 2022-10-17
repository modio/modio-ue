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

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/BaseWidgets/Slate/SModioCircularProgressBar.h"

#include "ModioCircularProgressBar.generated.h"

/**
* Modio UI element that that represent a progress bar, in other words
* the posibility to display progressive advances in a task
**/
UCLASS()
class MODIOUI_API UModioCircularProgressBar : public UWidget
{
	GENERATED_BODY()
protected:
	TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SModioCircularProgressBar> MyProgressBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0), Category = "Widgets")
	int32 BackgroundThickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category = "Widgets")
	int32 ForegroundThickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category = "Widgets")
	float Radius = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0, ClampMax = 360), Category = "Widgets")
	int32 StartAngleInDegrees = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0, ClampMax = 1), Category = "Widgets")
	float Progress = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FLinearColor BackgroundColor = FLinearColor::Black;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FLinearColor ForegroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, meta = (IsBindableEvent = "true"), Category = "Widgets")
	FGetFloat OnGetProgress;

	UFUNCTION(BlueprintNativeEvent, Category = "Widgets")
	float GetProgress() const;

public:
	/**
	* Get the current progress as a float
	* @return Float value for the progress reported by the delegate
	**/
	FGetFloat& GetProgressDelegate();
	
	/**
	* @brief Sets the progress value for this progress bar. Only works if the progress delegate is not bound
	* @param NewValue new progress value 0-1
	**/
	void SetProgress(float NewValue);

	/**
	*  Called by the editor to update modified state
	**/
	void SynchronizeProperties() override;
	
	/**
	* Clear resources in this Widget
	* @param bReleaseChildren Trigger release as well for super classes
	**/
	void ReleaseSlateResources(bool bReleaseChildren) override;
};
