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

#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_NEWER_THAN(5, 0, 0)
	#include "UObject/ObjectSaveContext.h"
#endif

#include "ModioProgressBar.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioProgressBar : public UProgressBar
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioProgressBarStyle"), Category = "Widget")
	FModioUIStyleRef StyleRef;

	virtual void SynchronizeProperties() override;
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#else
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "ModioProgressBar")
	void SetStyleRef(FModioUIStyleRef NewStyle);
};
