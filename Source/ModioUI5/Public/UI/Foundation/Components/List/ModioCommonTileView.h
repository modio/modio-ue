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
#include "CommonTileView.h"
#include "ModioCommonListViewStyle.h"
#include "ModioCommonTileView.generated.h"

/**
 * @brief Tile View that displays a list of tiles
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonTileView : public UCommonTileView
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the tile view within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonListViewStyle> InStyle);

protected:
	/** @brief The style of the tile view within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonListViewStyle> ModioStyle;

public:
	virtual void SynchronizeProperties() override;

protected:
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;
};
