#pragma once

#include "IModioModInfoUIDetails.h"
#include "Delegates/Delegate.h"
#include "Types/ModioErrorCode.h"
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/Widget.h"

#include "IModioUINotification.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioNotificationParams
{
	GENERATED_BODY()

	FFormatNamedArguments FormatArgs;

	TMap<FName, FText> NamedTextFormats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FModioErrorCode ErrorCode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* NotificationContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 1;
};

UCLASS(BlueprintType)
class UModioNotificationParamsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams CreateErrorNotificationParams(FModioErrorCode StatusCode, const FText& SuccessText,
																  const FText& ErrorText);
	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams CreateRatingNotification(FModioErrorCode StatusCode,
															 const TScriptInterface<IModioModInfoUIDetails>& ModInfo);
	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams CreateSubscriptionNotification(
		FModioErrorCode StatusCode);

	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams& AddFormatText(UPARAM(ref) FModioNotificationParams& NotificationParams,
												   const FName& Name, const FText& Text)
	{
		NotificationParams.NamedTextFormats.Add(Name, Text);
		return NotificationParams;
	}

	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams& SetTextArg(UPARAM(ref) FModioNotificationParams& NotificationParams,
												const FString& Name, const FText& Text)
	{
		NotificationParams.FormatArgs.Add(Name, Text);
		return NotificationParams;
	}

	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams& SetStringArg(UPARAM(ref) FModioNotificationParams& NotificationParams,
												  const FString& Name, const FString& Value)
	{
		NotificationParams.FormatArgs.Add(Name, FText::FromString(Value));
		return NotificationParams;
	}

	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams& SetFloatArg(UPARAM(ref) FModioNotificationParams& NotificationParams,
												 const FString& Name, float Value)
	{
		NotificationParams.FormatArgs.Add(Name, Value);
		return NotificationParams;
	}
	UFUNCTION(BlueprintCallable)
	static FModioNotificationParams& SetIntegerArg(UPARAM(ref) FModioNotificationParams& NotificationParams,
												   const FString& Name, int32 Value)
	{
		NotificationParams.FormatArgs.Add(Name, Value);
		return NotificationParams;
	}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnNotificationExpired, UWidget*, NotificationWidget);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUINotification : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUINotification : public IInterface
{
	GENERATED_BODY()
protected:
	bool bRoutedDisplayNotification = false;

	virtual void NativeOnDisplay() {};
	virtual void NativeConfigure(const FModioNotificationParams& Params) {};
	virtual void NativeSetNotificationExpireHandler(const FOnNotificationExpired& InDelegate) {};
	virtual UWidget* NativeGetAsWidget()
	{
		return nullptr;
	}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UWidget* GetAsWidget();

	UWidget* GetAsWidget_Implementation()
	{
		return NativeGetAsWidget();
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Configure(const FModioNotificationParams& Params);

	void Configure_Implementation(const FModioNotificationParams& Params)
	{
		NativeConfigure(Params);
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Display();

	void Display_Implementation()
	{
		NativeOnDisplay();
	}
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetNotificationExpireHandler(const FOnNotificationExpired& InDelegate);

	void SetNotificationExpireHandler_Implementation(const FOnNotificationExpired& InDelegate)
	{
		NativeSetNotificationExpireHandler(InDelegate);
	}
};
