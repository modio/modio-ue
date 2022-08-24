// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"
#include "Types/ModioErrorCode.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"

#if UE_VERSION_NEWER_THAN(5, 0, 0)
	#include "UObject/ObjectSaveContext.h"
#endif

#include "ModioUserWidgetBase.generated.h"

class FModioUserWidgetMetaData : public ISlateMetaData
{
protected:
	TWeakObjectPtr<class UModioUserWidgetBase> AssociatedWidget;

public:
	SLATE_METADATA_TYPE(FModioUserWidgetMetaData, ISlateMetaData)

	FModioUserWidgetMetaData(UModioUserWidgetBase* InWidget) : AssociatedWidget(InWidget) {};

	UModioUserWidgetBase* GetAssociatedWidget()
	{
		if (AssociatedWidget.IsValid())
		{
			return AssociatedWidget.Get();
		}
		return nullptr;
	}
};

/**
 *
 */
UCLASS()
class MODIOUI_API UModioUserWidgetBase : public UUserWidget, public IModioUIInputHandler
{
	GENERATED_BODY()
protected:
#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
											class IWidgetCompilerLog& CompileLog) const override;
#endif

	virtual void NativeConstruct() override;

	UPROPERTY(transient)
	bool bRoutedSetDataSource = false;
	UPROPERTY(transient)
	bool bRoutedLoadExternalData = false;
	UPROPERTY(transient)
	bool bRoutedFinishLoadExternalData = false;

	/// @brief Cached object to use as the source for data binding
	/// Will not be polled for state changes
	UPROPERTY(Transient, BlueprintReadOnly, EditAnywhere, Category = "Data Binding")
	UObject* DataSource;

	UFUNCTION()
	void FinishLoadExternalData(FModioErrorCode ec);

#if UE_VERSION_NEWER_THAN(5, 0, 0)
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#else
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
#endif

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Data Binding")
	void SetDataSource(UObject* NewDataSource);

	/// @brief Overrideable native event for data source changes
	virtual void NativeOnSetDataSource();
	/// @brief Blueprint event for data source changes. Invoked by NativeOnSetDataSource
	UFUNCTION(BlueprintImplementableEvent)
	void OnDataSourceUpdated();

	/// @brief Requests the widget make any network or asynchronous calls required to load external data to display
	UFUNCTION(BlueprintCallable, Category = "Data Binding")
	void LoadExternalData();
	/// @brief Overridable native event for the start of external data loading
	virtual void NativeBeginLoadExternalData();

	/// @brief Blueprint event invoked by NativeBeginLoadExternalData when external data starts to load. Implement this
	/// in widget blueprints to display loading indicators or other feedback.
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginLoadExternalData();

	/// @brief Native event when external data has completed loading
	/// @param ec ErrorCode describing the result of the external data load.
	virtual void NativeFinishLoadExternalData(FModioErrorCode ec);

	/// @brief Blueprint event for completion of external data load
	/// @param ec ErrorCode describing the result of the external data load. Use this value to decide what feedback to
	/// provide to users in widget blueprints (eg text indicating a network error)
	UFUNCTION(BlueprintImplementableEvent, Category = "Data Binding")
	void OnFinishLoadExternalData(FModioErrorCode ec);

	/// @brief Helper function for aliasing this UserWidget as the Widget at the root of it's WidgetTree
	/// @tparam WidgetType The type to cast to
	/// @return The root widget of this Userwidget's tree, cast to the specified type, or nullptr
	template<typename WidgetType>
	WidgetType* WidgetCast()
	{
		UWidget* CurrentRoot = GetRootWidget();
		if (CurrentRoot)
		{
			return Cast<WidgetType>(CurrentRoot);
		}
		return nullptr;
	}
};
