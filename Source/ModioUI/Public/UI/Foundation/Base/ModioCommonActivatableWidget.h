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

#include "CommonActivatableWidget.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ModioCommonActivatableWidget.generated.h"

class UModioCommonButtonBase;

/**
 * Base class for all Mod.io UI widgets that can have a data source and action bindings
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

	/** Whether the data source has been properly routed/forwarded to the NativeOnSetDataSource function */
	UPROPERTY(Transient)
	bool bRoutedSetDataSource = false;

public:
	/**
	 * Event that is invoked to synchronize properties
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Synchronize Properties"), Category = "Mod.io Common UI")
	void BP_SynchronizeProperties();

	/**
	 * Focuses on the desired widget obtained from GetDesiredFocusTarget
	 * @see NativeGetDesiredFocusTarget
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void FocusOnDesiredWidget();

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommonWidgetFocusChangedFast, bool /* bIsFocused */, UObject* /* Object */);
	/** Broadcasts when this or underlying widgets gain or lose focus */
	FOnCommonWidgetFocusChangedFast OnFocusChangedFast;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommonWidgetFocusChanged, bool, bIsFocused, UObject*, Object);
	/** Broadcasts when this or underlying widgets gain or lose focus. Suitable for Blueprints */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FOnCommonWidgetFocusChanged OnFocusChanged;

	/**
	 * Gets whether the widget should automatically bind input on Construct
	 * @return Whether the widget should automatically bind input on Construct
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	bool GetAutoBindInputAction() const { return bAutoBindInputAction; }

	/**
	 * Gets whether the widget should automatically focus on activation
	 * @return Whether the widget should automatically focus on activation
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	bool GetAutoFocusOnActivation() const { return bAutoFocusOnActivation; }

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ Begin UUserWidget Interface

protected:
	/** If true, this widget will automatically bind input on Construct and unbind on Destruct */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Properties")
	bool bAutoBindInputAction = true;

	/** If true, this widget will automatically focus on activation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Properties")
	bool bAutoFocusOnActivation = true;

protected:
	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	//~ End UCommonActivatableWidget Interface

	/**
	 * Event that is invoked when the viewport is resized
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Mod.io Common UI")
	void OnViewportResized(FVector2D NewViewportSize);

	/**
	 * Cached object to use as the source for data binding
	 * Will not be polled for state changes
	 */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Data Binding")
	TObjectPtr<UObject> DataSource;

	/**
	 * Overrideable native event for data source changes
	 */
	virtual void NativeOnSetDataSource();

	/**
	 * Blueprint event for data source changes. Invoked by NativeOnSetDataSource
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDataSourceUpdated();

public:
	/**
	 * Sets a new UObject as this widget's data source for data binding
	 * @param NewDataSource The new UObject to use
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Data Binding")
	void SetDataSource(UObject* NewDataSource);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Info")
	FString GetRequestIdentifier();

public:
	// TODO: These all functions for input binding may be moved to a controller class

	/**
	 * Called to bind input actions to this widget. It's called automatically on Construct if bAutoBindInputAction is true
	 * Override this function to bind your input
	 */
	virtual void BindInputActions();

	/**
	 * Called to bind input actions to this widget. It's called automatically on Construct if bAutoBindInputAction is true
	 * This is an alternative to BindInputActions suitable for Blueprints
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName = "Bind Input Actions", Category = "Mod.io Common UI|Input")
	void BP_BindInputActions();

	/**
	 * Called to unbind input actions from this widget. It's called automatically on Destruct
	 * Override this function to unbind your input
	 */
	virtual void UnbindInputActions();

	/**
	 * Called to unbind input actions from this widget
	 * This is an alternative to UnbindInputActions suitable for Blueprints
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName = "Unbind Input Actions", Category = "Mod.io Common UI|Input")
	void BP_UnbindInputActions();

	DECLARE_DYNAMIC_DELEGATE(FOnModioCommonActivatableWidgetActionFired);
	/**
	 * Listens for an input action and fires a callback when it's fired. Binds and unbinds on BindInputAction and UnbindInputAction
	 * This is an alternative to ListenForInputAction suitable for Blueprints
	 * @param Button Button to listen for input on. If null, will listen on this widget
	 * @param InputAction Input action to listen for
	 * @param DisplayName Display name of the input action
	 * @param OnActionFired Callback to fire when the input action is fired
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Listen For Input Action", Category = "Mod.io Common UI|Input")
	void BP_ListenForInputAction(UModioCommonButtonBase* Button, FDataTableRowHandle InputAction, const FText& DisplayName, const FOnModioCommonActivatableWidgetActionFired& OnActionFired);

	DECLARE_DELEGATE(FOnModioCommonActivatableWidgetActionFiredFast);
	/**
	 * Listens for an input action and fires a callback when it's fired. Binds and unbinds on BindInputAction and UnbindInputAction
	 * @param Button Button to listen for input on. If null, will listen on this widget
	 * @param InputAction Input action to listen for
	 * @param DisplayName Display name of the input action
	 * @param OnActionFired Callback to fire when the input action is fired
	 */
	virtual void ListenForInputAction(TObjectPtr<UModioCommonButtonBase> Button, FDataTableRowHandle InputAction, const FText& DisplayName, const FOnModioCommonActivatableWidgetActionFiredFast& OnActionFired);

	/**
	 * Clears all listening input actions
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Input")
	void ClearListeningInputActions();

	/**
	 * Clears a listening input action
	 * @param Button Button to clear the listening input action on
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Input")
	void ClearListeningInputAction(UModioCommonButtonBase* Button);

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	virtual void SynchronizeProperties() override;
	//~ End UUserWidget Interface
	
private:
	/** Information about a listening input action */
	struct FListeningInputActionStruct
	{
		FDataTableRowHandle InputAction;
		FText DisplayName;
		TWeakObjectPtr<UModioCommonButtonBase> Button;
		FOnModioCommonActivatableWidgetActionFiredFast OnActionFired;

		FListeningInputActionStruct() = delete;

		FListeningInputActionStruct(const FDataTableRowHandle& InInputAction, const FText& DisplayName, TWeakObjectPtr<UModioCommonButtonBase> InButton, const FOnModioCommonActivatableWidgetActionFiredFast& OnActionFired)
			: InputAction(InInputAction),
			  DisplayName(DisplayName),
			  Button(InButton),
			  OnActionFired(OnActionFired) {}
	};
	/** List of listening input actions. Added to in ListenForInputAction, cleared in ClearListeningInputActions */
	TArray<FListeningInputActionStruct> ListeningInputActions;

	/** Information about a bound input action */
	struct FBoundInputActionStruct
	{
		TWeakObjectPtr<UModioCommonButtonBase> Button;
		FUIActionBindingHandle ActionHandle;

		FBoundInputActionStruct() = delete;

		FBoundInputActionStruct(TWeakObjectPtr<UModioCommonButtonBase> InButton)
			: Button(InButton)
		{}

		FBoundInputActionStruct(FUIActionBindingHandle InActionHandle)
			: ActionHandle(InActionHandle)
		{}
	};

	/** List of bound input actions. Added to in BindInputAction, removed from in UnbindInputAction */
	TArray<FBoundInputActionStruct> BoundInputActions;
};
