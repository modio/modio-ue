/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "ModioSubsystem.h"
#include "UI/Foundation/Base/Report/ModioCommonReportViewBase.h"
#include "UObject/ObjectPtr.h"
#include "ModioCommonReportView.generated.h"

class UModioCommonWidgetSwitcher;
class UModioCommonReportReasonViewBase;
class UModioCommonReportEmailViewBase;
class UModioCommonReportMessageViewBase;
class UModioCommonReportSummaryViewBase;
struct FModioErrorCode;
struct FModioReportParams;

/**
 * Report View that displays a report reason view, email view and message view, with automatic switching between them
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonReportView : public UModioCommonReportViewBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonWidgetSwitcher> ReportSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonReportReasonViewBase> ReportReasonViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonReportEmailViewBase> ReportEmailViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonReportMessageViewBase> ReportMessageViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonReportSummaryViewBase> ReportSummaryViewClass;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeOnSetDataSource() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportReasonViewProceedClicked(EModioReportType Type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportReasonViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportEmailViewBackClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportEmailViewSubmitClicked(const FString& Email);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportEmailViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportMessageViewBackClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportMessageViewSubmitClicked(const FString& Message);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportMessageViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportSummaryViewBackClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportSummaryViewSubmitClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnReportSummaryViewCancelClicked();

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Report View|Data")
	FModioReportParams ReportParams;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleReportSubmit(FModioErrorCode ErrorCode);

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Report View|Widgets")
	TObjectPtr<UModioCommonReportReasonViewBase> ReportReasonView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Report View|Widgets")
	TObjectPtr<UModioCommonReportEmailViewBase> ReportEmailView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Report View|Widgets")
	TObjectPtr<UModioCommonReportMessageViewBase> ReportMessageView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Report View|Widgets")
	TObjectPtr<UModioCommonReportSummaryViewBase> ReportSummaryView;
};
