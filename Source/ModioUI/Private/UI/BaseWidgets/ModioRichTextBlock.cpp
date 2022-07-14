// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "RenderingThread.h"
#include "Settings/ModioUISettings.h"
#include "Styling/SlateStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"

template<class ObjectType>
struct FDeferredDeletor : public FDeferredCleanupInterface
{
public:
	FDeferredDeletor(ObjectType* InInnerObjectToDelete) : InnerObjectToDelete(InInnerObjectToDelete) {}

	virtual ~FDeferredDeletor()
	{
		delete InnerObjectToDelete;
	}

private:
	ObjectType* InnerObjectToDelete;
};

void UModioRichTextBlock::SynchronizeProperties()
{
	UpdateStyleData();
	Super::SynchronizeProperties();
}

TSharedRef<SWidget> UModioRichTextBlock::RebuildWidget()
{
	UpdateStyleData();

	TArray<TSharedRef<class ITextDecorator>> CreatedDecorators;
	CreateDecorators(CreatedDecorators);

	TSharedRef<FRichTextLayoutMarshaller> Marshaller = FRichTextLayoutMarshaller::Create(
		CreateMarkupParser(), CreateMarkupWriter(), CreatedDecorators, StyleInstance.Get());

	MyRichTextBlock =
		SNew(SModioRichTextBlock).StyleReference(&TextStyle).TextStyle(&DefaultTextStyle).Marshaller(Marshaller);

	return MyRichTextBlock.ToSharedRef();
}

UModioRichTextBlock::UModioRichTextBlock(const FObjectInitializer& Initializer) : URichTextBlock(Initializer)
{
	DecoratorClasses.AddUnique(UModioRichTextBlockDecorator::StaticClass());
}

void UModioRichTextBlock::UpdateStyleData()
{
	if (IsDesignTime())
	{
		InstanceDecorators.Reset();
	}

	StyleInstance = MakeShareable(new FSlateStyleSet(TEXT("RichTextStyle")), [](FSlateStyleSet* ObjectToDelete) {
		BeginCleanup(new FDeferredDeletor<FSlateStyleSet>(ObjectToDelete));
	});

	for (TSubclassOf<URichTextBlockDecorator> DecoratorClass : DecoratorClasses)
	{
		if (UClass* ResolvedClass = DecoratorClass.Get())
		{
			if (!ResolvedClass->HasAnyClassFlags(CLASS_Abstract))
			{
				URichTextBlockDecorator* Decorator = NewObject<URichTextBlockDecorator>(this, ResolvedClass);
				InstanceDecorators.Add(Decorator);
			}
		}
	}

	if (OnGetStyle.IsBound())
	{
		const FModioRichTextStyle& RichTextStyle = OnGetStyle.Execute();
		StyleInstance = RichTextStyle.GetStyleSet();
	}
	else
	{
		const FModioRichTextStyle* RichTextStyle = TextStyle.FindStyle<FModioRichTextStyle>();
		if (RichTextStyle)
		{
			StyleInstance = RichTextStyle->GetStyleSet();
		}
	}
	if (StyleInstance->HasWidgetStyle<FTextBlockStyle>(DefaultStyleName))
	{
		DefaultTextStyle = StyleInstance->GetWidgetStyle<FTextBlockStyle>(DefaultStyleName);
	}
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetDecoratorStyleSet(StyleInstance.Get());
		MyRichTextBlock->SetTextStyle(DefaultTextStyle);
	}
}

TArray<FString> UModioRichTextBlock::GetStyleNames() const
{
	TArray<FString> StyleNames;
	if (OnGetStyle.IsBound())
	{
		const FModioRichTextStyle& RichTextStyle = OnGetStyle.Execute();
		TArray<FName> TmpStyleNames;
		RichTextStyle.Styles.GenerateKeyArray(TmpStyleNames);
		for (const FName& Name : TmpStyleNames)
		{
			StyleNames.Add(Name.ToString());
		}
	}
	else
	{
		const FModioRichTextStyle* RichTextStyle = TextStyle.FindStyle<FModioRichTextStyle>();
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

/*

void UModioRichTextBlock::GetStyle_Implementation(FModioRichTextStyle& OutStyle)
{
	if (OnGetStyle.IsBound())
	{
		OutStyle = OnGetStyle.Execute();
	}
	else
	{
		UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();

		// Could we load the default style set?
		if (Settings && !Settings->DefaultStyleSet.IsNull())
		{
			UModioUIStyleSet* DefaultStyle = Settings->DefaultStyleSet.LoadSynchronous();
			if (DefaultStyle)
			{
				OutStyle = DefaultStyle->DefaultRichTextStyle;
			}
		}
	}
	OutStyle = FModioRichTextStyle::GetDefault();
}*/

FOnGetRichTextStyle& UModioRichTextBlock::GetStyleDelegate()
{
	return OnGetStyle;
}

void UModioRichTextBlock::AddDecorator(TSubclassOf<URichTextBlockDecorator> DecoratorClass)
{
	DecoratorClasses.Add(DecoratorClass);
}

void UModioRichTextBlock::SetDefaultStyleName(FName DefaultName /*= "default"*/)
{
	DefaultStyleName = DefaultName;
}
