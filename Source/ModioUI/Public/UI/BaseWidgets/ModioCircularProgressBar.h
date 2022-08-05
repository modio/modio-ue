// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/BaseWidgets/Slate/SModioCircularProgressBar.h"

#include "ModioCircularProgressBar.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioCircularProgressBar : public UWidget
{
	GENERATED_BODY()
protected:
	TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SModioCircularProgressBar> MyProgressBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0), Category="Widgets")
	int32 BackgroundThickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category="Widgets")
	int32 ForegroundThickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category="Widgets")
	float Radius = 10;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, meta = (ClampMin = 0, ClampMax = 360), Category="Widgets")
	int32 StartAngleInDegrees = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0, ClampMax = 1), Category="Widgets")
	float Progress = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FLinearColor BackgroundColor = FLinearColor::Black;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FLinearColor ForegroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, meta = (IsBindableEvent = "true"), Category="Widgets")
	FGetFloat OnGetProgress;

	UFUNCTION(BlueprintNativeEvent, Category="Widgets")
	float GetProgress() const ;

public:
	FGetFloat& GetProgressDelegate();
	/// @brief Sets the progress value for this progress bar. Only works if the progress delegate is not bound
	/// @param NewValue new progress value 0-1
	void SetProgress(float NewValue);
	void SynchronizeProperties() override;
	void ReleaseSlateResources(bool bReleaseChildren) override;
};
