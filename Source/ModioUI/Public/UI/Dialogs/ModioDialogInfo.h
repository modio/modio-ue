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
#include "Engine/DataAsset.h"
#include "Types/SlateEnums.h"
#include "UObject/NoExportTypes.h"
#include "Widgets/Layout/SScaleBox.h"

#include "ModioDialogInfo.generated.h"

/**
* Enum listing possible buttons in a dialog
**/
UENUM()
enum class EModioDialogButtonCommand : uint8
{
	/** A back button **/
	Back,

	/** A cancel button **/
	Cancel,

	/** A close button **/
	Close,

	/** A push dialog button **/
	PushDialog,

	/** An asynchronous operation dialog **/
	AsyncCallThenPushDialog,

	/** A push dialog with an operation button **/
	PushDialogWithOperation
};

/**
* Enum listing possible actions performed in a dialog
**/
UENUM()
enum class EModioDialogAsyncCall : uint8
{
	/** Send an email **/
	AuthSubmitEmail,

	/** Submit an authentication code **/
	AuthSubmitCode,

	/** Authenticate using an external source **/
	AuthSubmitExternal,

	/** Retrieve the Terms of Service **/
	FetchTermsOfService,

	/** Report a mod **/
	SubmitReport,

	/** Unsubscribe from a mod **/
	UnsubscribeFromMod,

	/** Remove a mod **/
	UninstallMod,

	/** Finish a current session **/
	Logout
};

/**
* Enum listing possible operations performed in a dialog
**/
UENUM()
enum class EModioDialogOperationCall : uint8
{
	/** Report using an email address **/
	SetReportEmailAddress,

	/** Report the details on the incident **/
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

/**
* Struct definition for the information display on a dialog button
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioDialogButtonInfo
{
	GENERATED_BODY()
	
	/**
	* Stored property for the label in the button
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FText ButtonLabel;

	/**
	* Stored property to represent the type of dialog button for this instance
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	EModioDialogButtonCommand ButtonCommand;

	/**
	* Stored property to represent the type of asynchronous call to perform
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::AsyncCallThenPushDialog",
					  EditConditionHides),
			  Category = "Widgets")
	EModioDialogAsyncCall AsyncCallType;

	/**
	* Stored property to represent the type of report operation
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::PushDialogWithOperation",
					  EditConditionHides),
			  Category = "Widgets")
	EModioDialogOperationCall OperationCallType;

	// clang-format off
	/**
	* Stored property to keep a reference to the receiver of the button action
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonCommand==EModioDialogButtonCommand::PushDialog||ButtonCommand==EModioDialogButtonCommand::AsyncCallThenPushDialog||ButtonCommand==EModioDialogButtonCommand::PushDialogWithOperation",EditConditionHides), Category="Widgets")
	UModioDialogInfo* Destination;
	// clang-format on
};

/**
* Modio UI element that defines what the information to display 
* on a dialog box
**/
UCLASS()
class MODIOUI_API UModioDialogInfo : public UObject
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the title text to display
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Widgets")
	FText TitleText;

	/**
	* Stored property widget reference of the content to display below the header
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSubclassOf<class UWidget> SubHeaderWidget;

	/**
	* Stored property of the horizontal alignment to the widget used below the header
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "SubHeaderWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> SubHeaderWidgetHAlign;

	/**
	* Stored property of the vertical alignment to the widget used below the header
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "SubHeaderWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> SubHeaderWidgetVAlign;

	/**
	* Stored property of the text to display in the dialog box
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true), Category = "Widgets")
	FText DialogText;

	/**
	* Stored property reference to the widget on the input field
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MustImplement = ModioUIStringInputWidget), Category = "Widgets")
	TSubclassOf<class UWidget> InputWidget;

	/**
	* Stored property of the horizontal alignment to the widget used as input
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> InputWidgetHAlign;

	/**
	* Stored property of the vertical alignment to the widget used as input
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> InputWidgetVAlign;

	/**
	* Stored property of the stretch direction to the widget used as input, with default as Both
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EStretchDirection::Type> InputWidgetScalingType = EStretchDirection::Both;

	/**
	* Stored property that defines the widget height of the input field
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	float InputWidgetHeightOverride;

	/**
	* Stored property that defines the text to show as hint on the input field
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "InputWidget!=nullptr", EditConditionHides),
			  Category = "Widgets")
	FText InputWidgetHintText;

	/**
	* Stored property reference to the area button to use
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSubclassOf<class UWidget> ButtonAreaWidget;

	/**
	* Stored property of the horizontal alignment to the widget used as area button
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget!=nullptr", EditConditionHides), Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> ButtonAreaWidgetHAlign;

	/**
	* Stored property of the vertical alignment to the widget used as area button 
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget!=nullptr", EditConditionHides), Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> ButtonAreaWidgetVAlign;

	/**
	* Stored property array with references to multiple dialog buttons
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "ButtonAreaWidget==nullptr", EditConditionHides), Category = "Widgets")
	TArray<FModioDialogButtonInfo> Buttons;
};
