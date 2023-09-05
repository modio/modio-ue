/*
 *  Copyright (C) 2022 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Layout/Margin.h"
#include "Misc/Attribute.h"
#include "Misc/EngineVersionComparison.h"
#include "Types/SlateEnums.h"
#include "UObject/SavePackage.h"

/// @brief Namespace containing helper methods to simplify UI code used in both UE4 and UE5
namespace ModioUIHelpers
{
	template<typename SlotClass>
	FORCEINLINE void SetHorizontalAlignment(SlotClass& Slot, EHorizontalAlignment NewAlignment)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		Slot.SetHorizontalAlignment(NewAlignment);
#else
		Slot.HAlign(NewAlignment);
#endif
	}

	template<typename SlotClass>
	FORCEINLINE EHorizontalAlignment GetHorizontalAlignment(SlotClass& Slot)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		return Slot.GetHorizontalAlignment();
#else
		return Slot.HAlignment;
#endif
	}

	template<typename SlotClass>
	FORCEINLINE void SetVerticalAlignment(SlotClass& Slot, EVerticalAlignment NewAlignment)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		Slot.SetVerticalAlignment(NewAlignment);
#else
		Slot.VAlign(NewAlignment);
#endif
	}

	template<typename SlotClass>
	FORCEINLINE EVerticalAlignment GetVerticalAlignment(SlotClass& Slot)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		return Slot.GetVerticalAlignment();
#else
		return Slot.VAlignment;
#endif
	}

	template<typename SlotClass>
	FORCEINLINE SlotClass& SetPadding(SlotClass& Slot, TAttribute<FMargin> NewPadding)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		Slot.SetPadding(MoveTemp(NewPadding));
#else
		Slot.Padding(NewPadding);
#endif
		return Slot;
	}

	template<typename SlotClass>
	FORCEINLINE SlotClass& SetPadding(SlotClass& Slot, float NewPadding)
	{
		return ModioUIHelpers::SetPadding(Slot, FMargin(NewPadding));
	}

	template<typename SlotClass>
	FORCEINLINE SlotClass& SetPadding(SlotClass& Slot, float Horizontal, float Vertical)
	{
		return ModioUIHelpers::SetPadding(Slot, FMargin(Horizontal, Vertical));
	}

	template<typename SlotClass>
	FORCEINLINE SlotClass& SetPadding(SlotClass& Slot, float Left, float Top, float Right, float Bottom)
	{
		return ModioUIHelpers::SetPadding(Slot, FMargin(Left, Top, Right, Bottom));
	}

	template<typename SlotClass>
	FORCEINLINE FMargin GetPadding(SlotClass& Slot)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		return Slot.GetPadding();
#else
		return Slot.SlotPadding.Get();
#endif
	}

	FORCEINLINE bool SavePackage(UPackage* InOuter, UObject* Base, EObjectFlags TopLevelFlags, const TCHAR* Filename)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		FSavePackageArgs Args;
		Args.TopLevelFlags = TopLevelFlags;
		return UPackage::SavePackage(InOuter, Base, Filename, Args);
#else
		return UPackage::SavePackage(InOuter, Base, TopLevelFlags, Filename);
#endif
	}

	template<typename WidgetClass, typename... WidgetSlotArgTypes>
	typename WidgetClass::FSlot* AddSlot(WidgetClass* Widget, WidgetSlotArgTypes&&... Args)
	{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		return Widget->AddSlot(std::forward<WidgetSlotArgTypes>(Args)...).GetSlot();
#else
		return &Widget->AddSlot(std::forward<WidgetSlotArgTypes>(Args)...);
#endif
	}
} // namespace ModioUIHelpers