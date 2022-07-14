// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/ModDetails/ModioModDetailsPropertyInspector.h"
#include "Blueprint/WidgetTree.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"

FText UModioModDetailsPropertyInspector::GetPropertyText(EModioModInfoPropertyType PropertyType)
{
	if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
	{
		switch (PropertyType)
		{
			case EModioModInfoPropertyType::CreatorName:
				return FText::FromString(ModInfo->Underlying.ProfileSubmittedBy.Username);
			case EModioModInfoPropertyType::LastUpdated:
			{
				FFormatNamedArguments Args;
				Args.Add("Days", (FDateTime::UtcNow() - ModInfo->Underlying.ProfileDateUpdated).GetDays());
				return FText::Format(LastUpdatedFormat, Args);
			}
			case EModioModInfoPropertyType::Name:
				return FText::FromString(ModInfo->Underlying.ProfileName);
			case EModioModInfoPropertyType::ReleaseDate:
				return FText::AsDate(ModInfo->Underlying.ProfileDateLive);
			case EModioModInfoPropertyType::Size:
				if (ModInfo->Underlying.FileInfo.Filesize < GB)
				{
					return UModioSDKLibrary::Filesize_ToString(ModInfo->Underlying.FileInfo.Filesize, 0);	
				}
			
				return UModioSDKLibrary::Filesize_ToString(ModInfo->Underlying.FileInfo.Filesize, 1);
			case EModioModInfoPropertyType::Subscribers:
				return FText::AsNumber(ModInfo->Underlying.Stats.SubscribersTotal);
		}
	}
	return FText::FromString("Invalid DataSource!");
}

TArray<FString> UModioModDetailsPropertyInspector::GetStyleNames(const FModioUIStyleRef& StyleRef) const
{
	TArray<FString> StyleNames;
	{
		const FModioRichTextStyle* RichTextStyle = StyleRef.FindStyle<FModioRichTextStyle>();
		if (RichTextStyle)
		{
			TArray<FName> TmpStyleNames;
			RichTextStyle->Styles.GenerateKeyArray(TmpStyleNames);
			for (const FName& Name : TmpStyleNames)
			{
				StyleNames.Add(Name.ToString());
			}
		}
	}
	if (StyleNames.Num() == 0)
	{
		StyleNames.Add("Default");
	}
	return StyleNames;
}

TArray<FString> UModioModDetailsPropertyInspector::GetPropertyNameStyleNames() const
{
	return GetStyleNames(PropertyNameTextStyle);
}

TArray<FString> UModioModDetailsPropertyInspector::GetPropertyValueStyleNames() const
{
	return GetStyleNames(PropertyValueTextStyle);
}

const FModioRichTextStyle& UModioModDetailsPropertyInspector::GetTextStyle(FModioUIStyleRef TextStyle)
{
	if (const FModioRichTextStyle* Style = TextStyle.FindStyle<FModioRichTextStyle>())
	{
		return *Style;
	}
	else
	{
		return FModioRichTextStyle::GetDefault();
	}
}

void UModioModDetailsPropertyInspector::NativeOnUpdatePropertyDisplay()
{
	UModioModPropertyInspectorBase::NativeOnUpdatePropertyDisplay();
	if (PropertyGrid && WidgetTree)
	{
		PropertyGrid->ClearChildren();
		int32 CurrentRowIndex = 0;
		for (const FModioModInfoPropertyDescriptor& PropDesc : PropertiesToDisplay)
		{
			UModioRichTextBlock* PropertyLabelBlock = WidgetTree->ConstructWidget<UModioRichTextBlock>();
			PropertyLabelBlock->SetDefaultStyleName(DefaultPropertyNameStyleName);
			PropertyLabelBlock->GetStyleDelegate().BindUObject(this, &UModioModDetailsPropertyInspector::GetTextStyle,
																 PropertyNameTextStyle);
			PropertyLabelBlock->SetText(PropDesc.Label);
			PropertyGrid->AddChildToGrid(PropertyLabelBlock, CurrentRowIndex, 0);

			UModioRichTextBlock* PropertyValueBlock = WidgetTree->ConstructWidget<UModioRichTextBlock>();
			PropertyValueBlock->SetDefaultStyleName(DefaultPropertyValueStyleName);
			PropertyValueBlock->GetStyleDelegate().BindUObject(this, &UModioModDetailsPropertyInspector::GetTextStyle,
																 PropertyValueTextStyle);
			PropertyValueBlock->SetText(GetPropertyText(PropDesc.Property));
			PropertyGrid->AddChildToGrid(PropertyValueBlock, CurrentRowIndex, 1);
			CurrentRowIndex++;
		}
	}
}
