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
#include "UI/Interfaces/IModioUINotificationController.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UObject/StrongObjectPtr.h"
#include "Containers/UnrealString.h"
#include "ModioCommonNotificationController.generated.h"

class UPanelWidget;

/**
 * Manual notification parameters
 */
struct FModioManualNotificationParams
{
	FModioManualNotificationParams() = default;
	FModioManualNotificationParams(const FText& InTitle, const FText& InMessage, bool bInIsError)
		: Title(InTitle),
		  Message(InMessage),
		  bIsError(bInIsError)
	{}
	bool operator==(const FModioManualNotificationParams& Other) const
	{
		return Title.EqualTo(Other.Title) && Message.EqualTo(Other.Message) && bIsError == Other.bIsError;
	}
	bool operator!=(const FModioManualNotificationParams& Other) const
	{
		return !(*this == Other);
	}
	friend uint32 GetTypeHash(const FModioManualNotificationParams& Other)
	{
		return GetTypeHash(Other.Title.ToString()) ^ GetTypeHash(Other.Message.ToString()) ^ GetTypeHash(Other.bIsError);
	}

	/**
	 * Title of the notification
	 */
	FText Title;

	/**
	 * Message of the notification
	 */
	FText Message;

	/**
	 * Whether the notification is an error
	 */
	bool bIsError;
};

/**
 * Notification Controller that displays notifications in a list
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonNotificationController
	: public UModioCommonActivatableWidget,
	  public IModioUINotificationController
{
	GENERATED_BODY()

public:
	/** Maximum number of notifications that can be displayed on screen at once */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI")
	int32 MaxNotificationsOnScreen = 5;

protected:
	//~ Begin UWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UWidget Interface

	/**
	 * Called when a notification is expired (the timer has run out)
	 * @param Notification The notification that expired
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnNotificationExpired(UWidget* Notification);

	//~ Begin IModioUINotificationController Interface
	virtual void NativeDisplayNotification(const TScriptInterface<class IModioUINotification>& Notification) override;
	virtual void NativeDisplayNotificationParams(const FModioNotificationParams& Params) override;
	virtual void NativeDisplayNotificationManual(const FText& Title, const FText& Message, bool bIsError) override;
	//~ End IModioUINotificationController Interface

	/** Error notification class to use when displaying an error */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "/Script/ModioUICore.ModioUINotification"), Category = "Mod.io Common UI")
	TSubclassOf<UUserWidget> ErrorNotificationClass;

	/** Success notification class to use when displaying a success */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "/Script/ModioUICore.ModioUINotification"), Category = "Mod.io Common UI")
	TSubclassOf<UUserWidget> SuccessNotificationClass;

	/** Notification list to display notifications in */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> NotificationList;

	/** This is needed to allow finding both by value and by key at O(1) */
	template<typename ParamsType>
	struct FBidirectionalWidgetMappedParams
	{
		const ParamsType* Find(const TStrongObjectPtr<UWidget>& Widget) const
		{
			return MappedWidgetToParams.Find(Widget);
		}
		const TStrongObjectPtr<UWidget>* Find(const ParamsType& Params) const
		{
			return MappedParamsToWidget.Find(Params);
		}
		void Add(const ParamsType& Params, const TStrongObjectPtr<UWidget>& Widget)
		{
			MappedParamsToWidget.Add(Params, Widget);
			MappedWidgetToParams.Add(Widget, Params);
		}
		void Remove(const ParamsType& Params)
		{
			if (const TStrongObjectPtr<UWidget>* FoundWidget = MappedParamsToWidget.Find(Params))
			{
				MappedWidgetToParams.Remove(*FoundWidget);
				MappedParamsToWidget.Remove(Params);
			}
		}
		void Remove(const TStrongObjectPtr<UWidget>& Widget)
		{
			if (const ParamsType* FoundParams = MappedWidgetToParams.Find(Widget))
			{
				MappedParamsToWidget.Remove(*FoundParams);
				MappedWidgetToParams.Remove(Widget);
			}
		}

		TMap<ParamsType, TStrongObjectPtr<UWidget>> MappedParamsToWidget;
		TMap<TStrongObjectPtr<UWidget>, ParamsType> MappedWidgetToParams;
	};

	/**
	 * Map of manual notification parameters to the notification widget
	 */
	FBidirectionalWidgetMappedParams<FModioManualNotificationParams> MappedManualParams;

	/**
	 * Map of notification parameters to the notification widget
	 */
	FBidirectionalWidgetMappedParams<FModioNotificationParams> MappedParams;

	/**
	 * @brief Checks if the same notification is already displayed and if so, moves it to the front of the list
	 * @param Params Notification parameters to check
	 * @return true if the notification was moved to the front of the list
	 */
	bool RefreshNotificationOrder(const FModioManualNotificationParams& Params);

	/**
	 * @brief Checks if the same notification is already displayed and if so, moves it to the front of the list
	 * @param Params Notification parameters to check
	 * @return true if the notification was moved to the front of the list
	 */
	bool RefreshNotificationOrder(const FModioNotificationParams& Params);

	/**
	 * @brief Moves the notification to the front of the list (and also nullifies the expiry timer)
	 * @param Notification Notification to move
	 */
	void MoveNotificationToFront(UWidget* Notification);
};
