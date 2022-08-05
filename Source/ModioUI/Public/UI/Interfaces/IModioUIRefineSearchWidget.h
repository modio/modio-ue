#pragma once
#include "Delegates/DelegateCombinations.h"
#include "Types/ModioFilterParams.h"
#include "UObject/Interface.h"

#include "IModioUIRefineSearchWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchSettingsChanged, FModioFilterParams, SearchSettings);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIRefineSearchWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIRefineSearchWidget : public IInterface
{
	GENERATED_BODY()
public:
protected:
	virtual FString NativeGetSearchString()
	{
		return FString {};
	};
	virtual TArray<FString> NativeGetSelectedTagValues()
	{
		return {};
	};
	virtual void NativeRefreshTags() {};

	virtual void NativeNotifySettingsChanged(const FModioFilterParams& SearchSettings)
	{
		bRoutedSearchSettingsChanged = true;
		NativeGetOnSettingsChangedDelegate().Broadcast(SearchSettings);
	}

	bool bRoutedSearchSettingsChanged = false;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIRefineSearchWidget")
	void NotifySettingsChanged(const FModioFilterParams& SearchSettings);

	void NotifySettingsChanged_Implementation(const FModioFilterParams& SearchSettings)
	{
		bRoutedSearchSettingsChanged = false;
		NativeNotifySettingsChanged(SearchSettings);
		checkf(bRoutedSearchSettingsChanged, TEXT("Please call IModioUIRefineSearchWidget::NotifySettingsChanged in "
												  "your overridden implementation to ensure events are routed"));
	}

	virtual FOnSearchSettingsChanged& NativeGetOnSettingsChangedDelegate() = 0;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIRefineSearchWidget")
	FString GetSearchString();

	FString GetSearchString_Implementation()
	{
		return NativeGetSearchString();
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIRefineSearchWidget")
	TArray<FString> GetSelectedTagValues();

	TArray<FString> GetSelectedTagValues_Implementation()
	{
		return NativeGetSelectedTagValues();
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIRefineSearchWidget")
	void RefreshTags();

	void RefreshTags_Implementation()
	{
		NativeRefreshTags();
	}

	virtual FOnSearchSettingsChanged& GetOnSettingsChangedDelegate()
	{
		return NativeGetOnSettingsChangedDelegate();
	};
};