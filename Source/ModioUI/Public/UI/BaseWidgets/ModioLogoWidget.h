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

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UObject/Object.h"

#include "ModioLogoWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioLogoWidget : public UWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UObject* DataSource;

protected:
	TSharedPtr<class SImage> MyImage;

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widgets")
	FModioUIStyleRef StyleRef = {FName("DefaultLogoTextStyle")};
};
