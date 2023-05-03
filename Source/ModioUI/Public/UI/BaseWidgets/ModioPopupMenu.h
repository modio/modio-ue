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

#include "Components/MenuAnchor.h"
#include "CoreMinimal.h"
#include "Framework/Text/TextLayout.h"
#include "UI/Commands/ModioUIMenuCommandList.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioPopupMenu.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioPopupMenu : public UMenuAnchor
{
	GENERATED_BODY()
protected:
	FModioUIMenuCommandList CurrentEntries;
	virtual void SynchronizeProperties() override;
	UFUNCTION()
	UUserWidget* GeneratePopupMenuContent();

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	UFUNCTION()
	virtual void HandleButtonClicked();

	void OnContentClose();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText ButtonLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TEnumAsByte<ETextJustify::Type> ButtonLabelJustification = ETextJustify::Center;

	UPROPERTY()
	class UUserWidget* CurrentContent;

public:
	UFUNCTION(BlueprintCallable, Category = "ModioPopupMenu")
	void SetMenuEntries(FModioUIMenuCommandList Entries);

	void SelectCurrentMenuItem();

	UPROPERTY()
	class UModioRichTextButton* MenuButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioButtonStyle", DesignerRebuild),
			  Category = "Widgets")
	FModioUIStyleRef ButtonStyle = FModioUIStyleRef {"DefaultButtonStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioPopupMenuStyle", DesignerRebuild),Category = "Widgets")
	FModioUIStyleRef PopupMenuStyle = FModioUIStyleRef {"DefaultPopupMenuStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "ModioUIPopupMenuContentWidget"),
			  Category = "Widgets")
	TSubclassOf<UUserWidget> MenuContentWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FKey KeyForInputHint;

	bool GetIsMenuOpen();
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool bPreviewOpened = false;
#endif
};
