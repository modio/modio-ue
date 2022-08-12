// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioComboBoxString.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Settings/ModioUISettings.h"
#include "Slate/SRetainerWidget.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

TSharedRef<SWidget> UModioComboBoxString::RebuildWidget()
{
	int32 InitialIndex = FindOptionIndex(GetSelectedOption());
	if (InitialIndex != -1)
	{
		CurrentOptionPtr = Options[InitialIndex];
	}

	UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
	// Could we load the default style set?
	FComboBoxStyle* ResolvedComboBoxStyle = nullptr;
	FModioWidgetBorderStyle* ResolvedMenuBorderStyle = nullptr;
	FModioWidgetBorderStyle* ResolvedButtonBorderStyle = nullptr;
	FTableRowStyle* ResolvedMenuItemStyle = nullptr;
	if (Settings && !Settings->DefaultStyleSet.IsNull())
	{
		UModioUIStyleSet* DefaultStyle = Settings->DefaultStyleSet.LoadSynchronous();
		ResolvedComboBoxStyle = &DefaultStyle->DefaultComboBoxStyle.ComboBoxStyle;
		ResolvedMenuBorderStyle = &DefaultStyle->DefaultComboBoxStyle.MenuBorderStyle;
		ResolvedButtonBorderStyle = &DefaultStyle->DefaultComboBoxStyle.ButtonBorderStyle;
		ResolvedMenuItemStyle = &DefaultStyle->DefaultComboBoxStyle.DropdownItemStyle;
	}
	else
	{
		ResolvedComboBoxStyle = &WidgetStyle;
		ResolvedMenuItemStyle = &ItemStyle;
	}

	MyModioComboBox = SNew(SModioComboBox<TSharedPtr<FString>>)
						  .ComboBoxStyle(ResolvedComboBoxStyle)
						  .ItemStyle(ResolvedMenuItemStyle)
						  .ForegroundColor(ForegroundColor)
						  .OptionsSource(&Options)
						  .InitiallySelectedItem(CurrentOptionPtr)
						  .ContentPadding(ContentPadding)
						  .MaxListHeight(MaxListHeight)
						  .HasDownArrow(HasDownArrow)
						  .EnableGamepadNavigationMode(EnableGamepadNavigationMode)
						  .OnGenerateWidget(SModioComboBox<TSharedPtr<FString>>::FOnGenerateWidget::CreateUObject(
							  this, &UModioComboBoxString::HandleGenerateWidget))
						  .OnSelectionChanged(BIND_UOBJECT_DELEGATE(
							  SModioComboBox<TSharedPtr<FString>>::FOnSelectionChanged, HandleSelectionChanged))
						  .OnComboBoxOpening(BIND_UOBJECT_DELEGATE(FOnComboBoxOpening, HandleOpening))
						  .Placement(Placement)
							.Method(bCreateNewWindow? EPopupMethod::CreateNewWindow : EPopupMethod::UseCurrentWindow)
						  .IsFocusable(bIsFocusable)[SAssignNew(ComboBoxContent, SBox)];
	MyComboBox = MyModioComboBox;
	if (InitialIndex != -1)
	{
		// Generate the widget for the initially selected widget if needed
		UpdateOrGenerateWidget(CurrentOptionPtr);
	}

	if (ResolvedMenuBorderStyle && ResolvedMenuBorderStyle->bMaskWithMaterial)
	{
		SAssignNew(MyWrappedContent, SRetainerWidget)[MyModioComboBox->GetUnderlyingMenuContent().ToSharedRef()];
		MyWrappedContent->SetEffectMaterial(ResolvedMenuBorderStyle->MaskMaterial.LoadSynchronous());
		MyWrappedContent->SetTextureParameter("WidgetTexture");
		UMaterialInstanceDynamic* WrapMaterialInstance = MyWrappedContent->GetEffectMaterial();
		ApplyModioBorderStyle(WrapMaterialInstance, ResolvedMenuBorderStyle);
		MyModioComboBox->SetMenuContent(MyWrappedContent.ToSharedRef());
	}

	if (ResolvedButtonBorderStyle && ResolvedButtonBorderStyle->bMaskWithMaterial)
	{
		SAssignNew(MyWrappedButton, SRetainerWidget)[MyModioComboBox.ToSharedRef()];
		MyWrappedButton->SetEffectMaterial(ResolvedButtonBorderStyle->MaskMaterial.LoadSynchronous());
		MyWrappedButton->SetTextureParameter("WidgetTexture");
		UMaterialInstanceDynamic* WrapMaterialInstance = MyWrappedButton->GetEffectMaterial();
		ApplyModioBorderStyle(WrapMaterialInstance, ResolvedButtonBorderStyle);
		return MyWrappedButton.ToSharedRef();
	}

	return MyComboBox.ToSharedRef();
}

void UModioComboBoxString::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
	// Could we load the default style set?
	//FComboBoxStyle* ResolvedComboBoxStyle = nullptr;
	FModioWidgetBorderStyle* ResolvedMenuBorderStyle = nullptr;
	FModioWidgetBorderStyle* ResolvedButtonBorderStyle = nullptr;
	if (Settings && !Settings->DefaultStyleSet.IsNull())
	{
		UModioUIStyleSet* DefaultStyle = Settings->DefaultStyleSet.LoadSynchronous();
		//ResolvedComboBoxStyle = &DefaultStyle->DefaultComboBoxStyle.ComboBoxStyle;
		ResolvedMenuBorderStyle = &DefaultStyle->DefaultComboBoxStyle.MenuBorderStyle;
		ResolvedButtonBorderStyle = &DefaultStyle->DefaultComboBoxStyle.ButtonBorderStyle;
	}
	else
	{
		//ResolvedComboBoxStyle = &WidgetStyle;
	}

	if (ResolvedMenuBorderStyle && ResolvedMenuBorderStyle->bMaskWithMaterial)
	{
		SAssignNew(MyWrappedContent, SRetainerWidget)[MyModioComboBox->GetUnderlyingMenuContent().ToSharedRef()];
		MyWrappedContent->SetEffectMaterial(ResolvedMenuBorderStyle->MaskMaterial.LoadSynchronous());
		MyWrappedContent->SetTextureParameter("WidgetTexture");
		UMaterialInstanceDynamic* WrapMaterialInstance = MyWrappedContent->GetEffectMaterial();
		ApplyModioBorderStyle(WrapMaterialInstance, ResolvedMenuBorderStyle);
		MyModioComboBox->SetMenuContent(MyWrappedContent.ToSharedRef());
	}
	else
	{
		MyWrappedContent.Reset();
	}

	if (ResolvedButtonBorderStyle && ResolvedButtonBorderStyle->bMaskWithMaterial)
	{
		SAssignNew(MyWrappedButton, SRetainerWidget)[MyModioComboBox.ToSharedRef()];
		MyWrappedButton->SetEffectMaterial(ResolvedButtonBorderStyle->MaskMaterial.LoadSynchronous());
		MyWrappedButton->SetTextureParameter("WidgetTexture");
		UMaterialInstanceDynamic* WrapMaterialInstance = MyWrappedButton->GetEffectMaterial();
		ApplyModioBorderStyle(WrapMaterialInstance, ResolvedButtonBorderStyle);
	}
	else
	{
		MyWrappedButton.Reset();
	}
}

void UModioComboBoxString::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyWrappedButton.Reset();
	MyWrappedContent.Reset();
	MyModioComboBox.Reset();
}

TSharedRef<SWidget> UModioComboBoxString::HandleGenerateWidget(TSharedPtr<FString> Item) const
{
	FString StringItem = Item.IsValid() ? *Item : FString();

	// Call the user's delegate to see if they want to generate a custom widget bound to the data source.
	if (OnGenerateWidgetEvent.IsBound())
	{
		UWidget* Widget = OnGenerateWidgetEvent.Execute(StringItem);
		if (Widget != NULL)
		{
			return Widget->TakeWidget();
		}
	}

	// If a row wasn't generated just create the default one, a simple text block of the item's name.
	return SNew(SBox).Padding(ContentPadding)[SNew(STextBlock).Text(FText::FromString(StringItem)).Font(Font)];
}
