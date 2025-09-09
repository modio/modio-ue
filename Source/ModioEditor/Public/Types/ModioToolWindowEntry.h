/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "ModioToolWindowEntry.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct MODIOEDITOR_API FModioToolWindowEntry
{
	GENERATED_BODY()

public:
	typedef TDelegate<TSharedPtr<SWidget>(class SModioEditorWindowCompoundWidget*)> CreateSubwindow;

	/* If false, this entry will not be displayed in the Tool Window Widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Tool Window Entry")
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Tool Window Entry")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Tool Window Entry")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Tool Window Entry")
	TSoftObjectPtr<UObject> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modio Tool Window Entry")
	bool bRequiresAuth;

	// Delegate that should be bound to create the widget when the category is clicked
	CreateSubwindow CreateWindowAction;

	FModioToolWindowEntry() : bEnabled(false), Name(), Description(), Icon(), bRequiresAuth(false), CreateWindowAction()
	{}

	FModioToolWindowEntry(bool bInEnabled, FText InName, FText InDescription, TSoftObjectPtr<UObject> InIcon,
						  CreateSubwindow InAction, bool bInRequiresAuth = false)
	{
		bEnabled = bInEnabled;
		Name = InName;
		Description = InDescription;
		Icon = InIcon;
		CreateWindowAction = InAction;
		bRequiresAuth = bInRequiresAuth;
	}

	FORCEINLINE bool operator==(const FModioToolWindowEntry& Other) const
	{
		return Name.EqualTo(Other.Name);
	}

	FORCEINLINE bool operator!=(const FModioToolWindowEntry& Other) const
	{
		return !Name.EqualTo(Other.Name);
	}

	FORCEINLINE friend uint32 GetTypeHash(const FModioToolWindowEntry& Entry)
	{
		return HashCombine(GetTypeHash(Entry.Name.ToString()), GetTypeHash(Entry.bEnabled));
	}
};
