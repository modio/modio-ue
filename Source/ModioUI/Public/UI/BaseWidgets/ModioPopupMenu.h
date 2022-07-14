// Fill out your copyright notice in the Description page of Project Settings.

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
	UWidget* GeneratePopupMenuContent();

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	UFUNCTION()
	virtual void HandleButtonClicked();

	void OnContentClose();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ButtonLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<ETextJustify::Type> ButtonLabelJustification = ETextJustify::Center;

	UPROPERTY(Transient)
	class UModioRichTextButton* MenuButton;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioButtonStyle", DesignerRebuild))
	FModioUIStyleRef ButtonStyle = FModioUIStyleRef {"DefaultButtonStyle"};

	UFUNCTION(BlueprintCallable)
	void SetMenuEntries(FModioUIMenuCommandList Entries);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "ModioUIPopupMenuContentWidget"))
	TSubclassOf<UWidget> MenuContentWidgetClass;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bPreviewOpened = false;
#endif
};
