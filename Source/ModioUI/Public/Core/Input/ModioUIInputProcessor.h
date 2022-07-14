#pragma once
#include "Core/Input/InputDeviceTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "Framework/Application/IInputProcessor.h"
#include "Input/Events.h"
#include "Misc/Optional.h"

/// @brief Class responsible for detecting input mode changes so that input hints, etc, know what type of device is
/// currently being used. This base class treats all controllers as Xbox controllers unless
/// UModioUISubsystem::SetControllerOverrideType is called by your application
/// Alternatively, subclass FModioUIInputProcessor, construct your derived type in UModioUISubsystem::Initialize and
/// override DetectDeviceType to leverage RawInput or WinDualShock plugins to have the UI handle that scenario
/// automatically
class MODIOUI_API FModioUIInputProcessor : public IInputProcessor
{
	EModioUIInputMode CurrentInputMode = EModioUIInputMode::Unknown;

	TOptional<EModioUIInputMode> PendingInputMode;

	TOptional<EModioUIInputMode> CurrentControllerTypeOverride;

	FOnInputDeviceChanged OnInputModeChangedDelegate;

	void HandleInputModeChange(EModioUIInputMode NewMode)
	{
		if (CurrentInputMode != NewMode)
		{
			PendingInputMode = NewMode;
		}
		else
		{
			PendingInputMode.Reset();
		}
	}
	// Actually performing the broadcast here allows us to accumulate multiple changes of input device over a frame and
	// only broadcast the most recent one when Tick is called
	void BroadcastInputModeChange()
	{
		if (PendingInputMode.IsSet())
		{
			CurrentInputMode = PendingInputMode.GetValue();
			OnInputModeChangedDelegate.Broadcast(CurrentInputMode);
			PendingInputMode.Reset();
		}
	}

protected:
	virtual EModioUIInputMode DetectDeviceType(const FKeyEvent& InEvent);
	virtual bool ShouldEmitDeviceChangeEventsForKeyEvent(const FKeyEvent& InEvent);

public:
	void SetControllerOverrideType(EModioUIInputMode Override);

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		BroadcastInputModeChange();
	}

	FOnInputDeviceChanged& OnInputDeviceChanged()
	{
		return OnInputModeChangedDelegate;
	}

	bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (ShouldEmitDeviceChangeEventsForKeyEvent(InKeyEvent))
		{
			HandleInputModeChange(DetectDeviceType(InKeyEvent));
		}
		return false;
	}

	bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (ShouldEmitDeviceChangeEventsForKeyEvent(InKeyEvent))
		{
			HandleInputModeChange(DetectDeviceType(InKeyEvent));
		}
		return false;
	}

	bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override
	{
		const FKeyEvent KeyEvent = InAnalogInputEvent;
		HandleInputModeChange(DetectDeviceType(KeyEvent));

		return false;
	}

	bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleInputModeChange(EModioUIInputMode::Mouse);
		return false;
	}

	bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleInputModeChange(EModioUIInputMode::Mouse);
		return false;
	}

	bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleInputModeChange(EModioUIInputMode::Mouse);
		return false;
	}

	bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleInputModeChange(EModioUIInputMode::Mouse);
		return false;
	}

	bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent,
										const FPointerEvent* InGestureEvent) override
	{
		HandleInputModeChange(EModioUIInputMode::Mouse);
		return false;
	}
};