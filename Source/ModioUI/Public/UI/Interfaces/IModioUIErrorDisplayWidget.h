#pragma once

#include "UObject/Interface.h"
#include "UObject/WeakInterfacePtr.h"
#include "Widgets/Notifications/SErrorText.h"
#include "Types/ModioErrorCode.h"
#include "Misc/EngineVersionComparison.h"

#include "IModioUIErrorDisplayWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIErrorDisplayWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIErrorDisplayWidget : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeDisplayError(const FModioErrorCode& ec)
		PURE_VIRTUAL(IModioUIErrorDisplayWidget::NativeDisplayError, );

	virtual void NativeSetErrorString(const FString& InErrorString)
	{
		NativeSetErrorText(FText::FromString(InErrorString));
	}

	virtual void NativeSetErrorText(const FText& InErrorText) {}
	virtual bool NativeIsErrorSet() const
	{
		return false;
	}

	/// Non virtual - please override the Native* methods instead
	void SetError(const FString& InErrorString)
	{
		SetErrorString(InErrorString);
	};
	void SetError(const FText& InErrorText)
	{
		SetErrorText(InErrorText);
	};
	bool HasError() const
	{
		return IsErrorSet();
	};

	class ModioErrorReportingWidgetWrapper : public IErrorReportingWidget
	{
		TWeakInterfacePtr<IModioUIErrorDisplayWidget> ReferencedWidget;

	public:
#if UE_VERSION_NEWER_THAN(4, 27, 0)
		ModioErrorReportingWidgetWrapper(IModioUIErrorDisplayWidget* WrappedObject) 
		{
			ReferencedWidget = TWeakInterfacePtr<IModioUIErrorDisplayWidget>(WrappedObject);
		}
#else
		// 4.26 doesn't quite support what we need, so we'll just do it the old way
		ModioErrorReportingWidgetWrapper(IModioUIErrorDisplayWidget* WrappedObject) : ReferencedWidget(*WrappedObject)
			{}
#endif
		virtual void SetError(const FString& InErrorString)
		{
			if (ReferencedWidget.IsValid())
			{
				ReferencedWidget->SetErrorString(InErrorString);
			}
		}
		virtual void SetError(const FText& InErrorText)
		{
			if (ReferencedWidget.IsValid())
			{
				ReferencedWidget->SetErrorText(InErrorText);
			}
		}

		virtual bool HasError() const
		{
			if (ReferencedWidget.IsValid())
			{
				return ReferencedWidget->IsErrorSet();
			}
			return false;
		}
		virtual TSharedRef<SWidget> AsWidget()
		{
			if (ReferencedWidget.IsValid())
			{
				return ReferencedWidget->AsWidget();
			}
			return SNullWidget::NullWidget;
		};
	};

public:
	TSharedRef<SWidget> AsWidget()
		PURE_VIRTUAL("IModioUIErrorDisplayWidget::AsWidget", return SNullWidget::NullWidget;);

	TSharedPtr<IErrorReportingWidget> AsErrorReportingWidget()
	{
		return MakeShared<ModioErrorReportingWidgetWrapper>(this);
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIErrorDisplayWidget")
	void SetErrorString(const FString& InErrorString);

	void SetErrorString_Implementation(const FString& InErrorString)
	{
		NativeSetErrorString(InErrorString);
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIErrorDisplayWidget")
	void SetErrorText(const FText& InErrorText);

	void SetErrorText_Implementation(const FText& InErrorText)
	{
		NativeSetErrorText(InErrorText);
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIErrorDisplayWidget")
	bool IsErrorSet() const;

	bool IsErrorSet_Implementation() const
	{
		return NativeIsErrorSet();
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIErrorDisplayWidget")
	void DisplayError(const FModioErrorCode& ec);

	void DisplayError_Implementation(const FModioErrorCode& ec)
	{
		NativeDisplayError(ec);
	}

	// Needs an onclosed handler so we know when it's closed
};