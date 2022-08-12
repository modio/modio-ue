// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/SlateEnums.h"
#include "UObject/NoExportTypes.h"
#include "Widgets/Layout/SScaleBox.h"

#include "ModioDialogInfo.generated.h"

UENUM()
enum class EModioDialogButtonCommand : uint8
{
	Back,
	Cancel,
	Close,
	PushDialog,
	AsyncCallThenPushDialog,
	PushDialogWithOperation
};
UENUM()
enum class EModioDialogAsyncCall : uint8
{
	AuthSubmitEmail,
	AuthSubmitCode,
	AuthSubmitExternal,
	FetchTermsOfService,
	SubmitReport,
	UnsubscribeFromMod,
	UninstallMod,
	Logout
};

UENUM()
enum class EModioDialogOperationCall : uint8
{
	SetReportEmailAddress,
	SetReportDetails
};

// need some kind of extended action thing here beyond just pushing a dialog
// auth_email
// auth_code
// auth_steam
// terms_of_service
// submit_report

// for the extended dialogs, these are all async actions, so we can have a generic error handler
// generic handler will try the action, then show a dialog if there was a problem

USTRUCT(BlueprintType)
struct MODIOUI_API FModioDialogButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FText ButtonLabel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	EModioDialogButtonCommand ButtonCommand;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::AsyncCallThenPushDialog",
					  EditConditionHides),
			  Category = "Widgets")
	EModioDialogAsyncCall AsyncCallType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::PushDialogWithOperation",
					  EditConditionHides),
			  Category = "Widgets")
	EModioDialogOperationCall OperationCallType;

	// clang-format off
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::PushDialog||ButtonCommand==EModioDialogButtonCommand::AsyncCallThenPushDialog||ButtonCommand==EModioDialogButtonCommand::PushDialogWithOperation",EditConditionHides), Category="Widgets")
	UModioDialogInfo* Destination;
	// clang-format on
};

/**
 *
 */
UCLASS()
class MODIOUI_API UModioDialogInfo : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Widgets")
	FText TitleText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSubclassOf<class UWidget> SubHeaderWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "SubHeaderWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> SubHeaderWidgetHAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "SubHeaderWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> SubHeaderWidgetVAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Widgets")
	FText DialogText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MustImplement = ModioUIStringInputWidget), Category = "Widgets")
	TSubclassOf<class UWidget> InputWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> InputWidgetHAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> InputWidgetVAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EStretchDirection::Type> InputWidgetScalingType = EStretchDirection::Both;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	float InputWidgetHeightOverride;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	FText InputWidgetHintText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSubclassOf<class UWidget> ButtonAreaWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget!=nullptr", EditConditionHides), Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> ButtonAreaWidgetHAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget!=nullptr", EditConditionHides), Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> ButtonAreaWidgetVAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget==nullptr", EditConditionHides), Category = "Widgets")
	TArray<FModioDialogButtonInfo> Buttons;
};
