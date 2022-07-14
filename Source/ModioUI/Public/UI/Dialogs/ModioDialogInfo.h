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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ButtonLabel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EModioDialogButtonCommand ButtonCommand;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::AsyncCallThenPushDialog",
					  EditConditionHides))
	EModioDialogAsyncCall AsyncCallType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::PushDialogWithOperation",
					  EditConditionHides))
	EModioDialogOperationCall OperationCallType;

	// clang-format off
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::PushDialog||ButtonCommand==EModioDialogButtonCommand::AsyncCallThenPushDialog||ButtonCommand==EModioDialogButtonCommand::PushDialogWithOperation",EditConditionHides))
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true))
	FText TitleText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UWidget> SubHeaderWidget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "SubHeaderWidget!=nullptr", EditConditionHides))
    TEnumAsByte<EHorizontalAlignment> SubHeaderWidgetHAlign;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "SubHeaderWidget!=nullptr", EditConditionHides))
    TEnumAsByte<EVerticalAlignment> SubHeaderWidgetVAlign;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true))
	FText DialogText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MustImplement = ModioUIStringInputWidget))
	TSubclassOf<class UWidget> InputWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides))
	TEnumAsByte<EHorizontalAlignment> InputWidgetHAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides))
	TEnumAsByte<EVerticalAlignment> InputWidgetVAlign;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides))
	TEnumAsByte<EStretchDirection::Type> InputWidgetScalingType = EStretchDirection::Both;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides))
	float InputWidgetHeightOverride;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides))
	FText InputWidgetHintText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UWidget> ButtonAreaWidget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ButtonAreaWidget!=nullptr", EditConditionHides))
	TEnumAsByte<EHorizontalAlignment> ButtonAreaWidgetHAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ButtonAreaWidget!=nullptr", EditConditionHides))
	TEnumAsByte<EVerticalAlignment> ButtonAreaWidgetVAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget==nullptr", EditConditionHides))
	TArray<FModioDialogButtonInfo> Buttons;
};
