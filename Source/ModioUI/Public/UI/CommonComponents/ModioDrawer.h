// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/SlateEnums.h"
#include "UI/BaseWidgets/FxWidget.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioDrawer.generated.h"
/**
 *
 */
UCLASS()
class MODIOUI_API UModioDrawer : public UWidget, public INamedSlotInterface
{
	GENERATED_BODY()
protected:
	TSharedPtr<class SGridPanel> MyGridPanel;
	TSharedPtr<class SBorder> MyBorder;
	TSharedPtr<class SBox> MyDrawerBox;
	static const FName HeaderName;
	static const FName ContentName;
	static const FName FooterName;
	// UNamedSlotInterface Begin
	virtual void GetSlotNames(TArray<FName>& SlotNames) const override;
	virtual UWidget* GetContentForSlot(FName SlotName) const override;
	virtual void SetContentForSlot(FName SlotName, UWidget* Content) override;
	// UNamedSlotInterface End
	TSharedRef<SWidget> GetHeaderWidget();
	TSharedRef<SWidget> GetContentWidget();
	TSharedRef<SWidget> GetFooterWidget();
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual FReply OnMouseUp(const FGeometry&, const FPointerEvent&);

	UPROPERTY()
	TMap<FName, UWidget*> NamedWidgets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioDrawerWidgetStyle"), Category="Widgets")
	FModioUIStyleRef DrawerStyle;

	UPROPERTY()
	class UBorder* HeaderBorder;

	UPROPERTY()
	class UBorder* ContentBorder;

	UPROPERTY()
	class UBorder* FooterBorder;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fill Rules")
	float HeaderFillOverride = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fill Rules")
	float ContentFillOverride = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fill Rules")
	float FooterFillOverride = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Header Slot", meta = (DisplayName = "Padding"))
	FMargin HeaderPadding;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Header Slot", meta = (DisplayName = "Horizontal Alignment"))
	TEnumAsByte<EHorizontalAlignment> HeaderHorizontalAlignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Header Slot", meta = (DisplayName = "Vertical Alignment"))
	TEnumAsByte<EVerticalAlignment> HeaderVerticalAlignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Content Slot", meta = (DisplayName = "Padding"))
	FMargin ContentPadding;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Content Slot", meta = (DisplayName = "Horizontal Alignment"))
	TEnumAsByte<EHorizontalAlignment> ContentHorizontalAlignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Content Slot", meta = (DisplayName = "Vertical Alignment"))
	TEnumAsByte<EVerticalAlignment> ContentVerticalAlignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Footer Slot", meta = (DisplayName = "Padding"))
	FMargin FooterPadding;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Footer Slot", meta = (DisplayName = "Horizontal Alignment"))
	TEnumAsByte<EHorizontalAlignment> FooterHorizontalAlignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Footer Slot", meta = (DisplayName = "Vertical Alignment"))
	TEnumAsByte<EVerticalAlignment> FooterVerticalAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fill Rules")
	int MinWidth;

	// TODO: expose direction for animation?
};
