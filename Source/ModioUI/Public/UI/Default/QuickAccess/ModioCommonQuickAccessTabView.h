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
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/Foundation/Base/QuickAccess/ModioCommonQuickAccessViewBase.h"
#include "ModioCommonQuickAccessTabView.generated.h"

class UModioCommonBorder;
class UModioCommonTextBlock;
class UModioCommonDynamicImage;
class UModioCommonButtonBase;
class UModioCommonModListView;
class UModioCommonModOperationTrackerWidget;
class UModioCommonModOperationTrackerUserWidget;
class UModioCommonProfileImage;
class UModioCommonQuickAccessTabViewStyle;
class UListView;


/**
 * Quick Access View that displays a tabbed view of the user's information, such as their profile image, storage space, and mod management
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonQuickAccessTabView
	: public UModioCommonQuickAccessViewBase,
	  public IModioUIModManagementEventReceiver,
	  public IModioUISubscriptionsChangedReceiver
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Quick Access View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonQuickAccessTabViewStyle> InStyle);

protected:
	/** The style of the Quick Access View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonQuickAccessTabViewStyle> ModioStyle;

	/** Internal background border, used to display the background beyond the actual tab */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> InternalBackgroundBorder;

	/** Overlay background border, used to display the background over the actual tab */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonBorder> OverlayBackgroundBorder;

	/** The user's profile image */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonProfileImage> ProfileImage;

	/** The user's name */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> UserNameTextBlock;

	/** The mod operation tracker user widget. Can be used to track and display the status of mod operations as a standalone widget */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonModOperationTrackerUserWidget> ModOperationTrackerUserWidget;

	/** Button that forwards to the main game menu (such as closing the mod.io-specific mod browser and returning to the main game menu) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> MainGameMenuButton;

	/** Button that forwards to the authentication view */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> AuthButton;

	/** Button that forwards to the user's collection */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> MyCollectionButton;

public:
	UModioCommonQuickAccessTabView();

protected:
	/**
	 * Called when the user clicks the close button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCloseButtonClicked();

	/**
	 * Called when the user clicks the main game menu button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnMainGameMenuButtonClicked();

	/**
	 * Called when the user clicks the auth button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnAuthButtonClicked();

	/**
	 * Called when the user clicks the collection button
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCollectionButtonClicked();

protected:
	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
};
