// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#if WITH_EDITOR
	#include "Blueprint/WidgetTree.h"
	#include "Editor/WidgetCompilerLog.h"
#endif

void UModioUserWidgetBase::SetDataSource(UObject* NewDataSource)
{
	bRoutedSetDataSource = false;

	DataSource = NewDataSource;

	// forward to our overrideable native implementation
	NativeOnSetDataSource();

	// Verify that the derived implementation called our base implementation
	checkf(bRoutedSetDataSource, TEXT("Please ensure you call Super::NativeOnSetDataSource in your overridden "
									  "implementation to route notifications to Blueprint"));
}

void UModioUserWidgetBase::NativeOnSetDataSource()
{
	bRoutedSetDataSource = true;
	OnDataSourceUpdated();
}

void UModioUserWidgetBase::LoadExternalData()
{
	bRoutedLoadExternalData = false;

	NativeBeginLoadExternalData();

	// Verify that the derived implementation called our base implementation
	checkf(bRoutedLoadExternalData, TEXT("Please ensure you call Super::NativeBeginLoadExternalData in your overridden "
										 "implementation to route notifications to Blueprint"));
}

void UModioUserWidgetBase::NativeBeginLoadExternalData()
{
	bRoutedLoadExternalData = true;
	OnBeginLoadExternalData();
}

#if WITH_EDITOR
void UModioUserWidgetBase::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
													  class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
	for (TFieldIterator<FProperty> PropIterator(GetClass()); PropIterator; ++PropIterator)
	{
		FObjectProperty* ObjectProperty = CastField<FObjectProperty>(*PropIterator);
		if (ObjectProperty && ObjectProperty->PropertyClass && ObjectProperty->PropertyClass->IsChildOf<UWidget>())
		{
			if ((ObjectProperty->HasMetaData("BindWidget") || ObjectProperty->HasMetaData("BindWidgetOptional")) &&
				ObjectProperty->HasMetaData("MustImplement"))
			{
				UWidget* Widget = BlueprintWidgetTree.FindWidget(ObjectProperty->GetFName());
				if (Widget &&
					!Widget->GetClass()->ImplementsInterface(ObjectProperty->GetClassMetaData("MustImplement")))
				{
					CompileLog.Error(
						FText::Format(FTextFormat(FText::FromString("{0} does not implement required interface {1}")),
									  FText::FromName(ObjectProperty->GetFName()),
									  FText::FromName(ObjectProperty->GetClassMetaData("MustImplement")->GetFName())));
				}
			}
		}
	}
}
#endif

void UModioUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	TSharedRef<SWidget> ConstructedWidget = TakeWidget();
	ConstructedWidget->AddMetadata(MakeShared<FModioUserWidgetMetaData>(this));
}

void UModioUserWidgetBase::FinishLoadExternalData(FModioErrorCode ec)
{
	bRoutedFinishLoadExternalData = false;

	NativeFinishLoadExternalData(ec);

	// Verify that the derived implementation called our base implementation
	checkf(bRoutedFinishLoadExternalData,
		   TEXT("Please ensure you call Super::NativeFinishLoadExternalData in your overridden "
				"implementation to route notifications to Blueprint"));
}
#if UE_VERSION_NEWER_THAN(5, 0, 0)
void UModioUserWidgetBase::PreSave(FObjectPreSaveContext SaveContext)
#else
void UModioUserWidgetBase::PreSave(const class ITargetPlatform* TargetPlatform)
#endif
{
	for (FPropertyValueIterator It = FPropertyValueIterator(FStructProperty::StaticClass(), GetClass(), this); It; ++It)
	{
		const FStructProperty* Prop = CastField<FStructProperty>(It->Key);

		const void* Value = It->Value;
		if (Prop->Struct == FSlateColor::StaticStruct())
		{
			FSlateColor* ActualColor = const_cast<FSlateColor*>(reinterpret_cast<const FSlateColor*>(Value));
			if (!ActualColor->IsColorSpecified())
			{
				ActualColor->Unlink();
			}
		}
	}

#if UE_VERSION_NEWER_THAN(5, 0, 0)
	Super::PreSave(SaveContext);
#else
	Super::PreSave(TargetPlatform);
#endif
}

void UModioUserWidgetBase::NativeFinishLoadExternalData(FModioErrorCode ec)
{
	bRoutedFinishLoadExternalData = true;
	OnFinishLoadExternalData(ec);
}

FReply UModioUserWidgetBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (ProcessCommandForEvent(InKeyEvent))
	{
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}