/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioUIEditor.h"
#include "AssetActions/AssetTypeActions_ModioDialogInfo.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Components/Widget.h"
#include "ContentBrowserModule.h"
#include "Editor/ModioModInfoUIFactory.h"
#include "Editor/ModioUIContentBrowserExtensions.h"
#include "IAssetTools.h"
#include "ISettingsModule.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"
#include "Settings/ModioUISettings.h"
#include "Settings/ModioUISettingsDetailsCustomization.h"
#include "Styling/SlateBrush.h"
#include "UI/BaseWidgets/ModioInputBindingImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Input/ModioKeyDetailsCustomization.h"
#include "UI/Styles/ModioSlateBrushDetailsCustomization.h"
#include "UI/Styles/ModioTextBlockStyleOverride.h"
#include "UI/Styles/ModioTextBlockStyleOverrideDetailsCustomization.h"
#include "UI/Styles/ModioUIBrushRefDetailsCustomization.h"
#include "UI/Styles/ModioUIColorDetailsCustomization.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIStyleRefDetailsCustomization.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UI/Styles/ModioUIStyleSetDetailsCustomization.h"
#include "UObject/UObjectIterator.h"

#include "Editor/ModioEditorHelpers.h"

DEFINE_LOG_CATEGORY(ModioUIEditor);

#define LOCTEXT_NAMESPACE "FModioUIEditor"

void FModioUIEditor::StartupModule()
{
	RegisterSettings();
	RegisterDetailsCustomizations();
	RegisterAssetActions();
}

void FModioUIEditor::ShutdownModule()
{
	UnregisterSettings();
	UnregisterDetailsCustomizations();
	UnregisterAssetActions();
}

void FModioUIEditor::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "mod.io UI", LOCTEXT("UISettingsName", "mod.io UI"),
										 LOCTEXT("UISettingsDescription", "Configure the mod.io plugin in-game UI"),
										 GetMutableDefault<UModioUISettings>());
	}
}

void FModioUIEditor::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "mod.io UI");
	}
}

void FModioUIEditor::RegisterDetailsCustomizations()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// to register our custom property
	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomPropertyTypeLayoutNames.Add_GetRef(FModioInputMapping::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioInputMappingCustomization>));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomPropertyTypeLayoutNames.Add_GetRef(FModioUIColor::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioUIColorDetailsCustomization>));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomPropertyTypeLayoutNames.Add_GetRef(FModioUIColorRef::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioUIColorRefDetailsCustomization>));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomPropertyTypeLayoutNames.Add_GetRef(FModioUIStyleRef::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioUIStyleRefDetailsCustomization>));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomPropertyTypeLayoutNames.Add_GetRef(FModioUIMaterialRef::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioUIMaterialRefDetailsCustomization>));

	PropertyModule.RegisterCustomClassLayout(
		CustomClassLayoutNames.Add_GetRef(UModioUIStyleSet::StaticClass()->GetFName()),
		FOnGetDetailCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakeDetailCustomizationInstance<FModioUIStyleSetDetailsCustomization>));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomClassLayoutNames.Add_GetRef(FSlateBrush::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioSlateBrushStructCustomization>));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		CustomClassLayoutNames.Add_GetRef(FModioTextBlockStyleOverride::StaticStruct()->GetFName()),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioTextBlockStyleOverrideDetailsCustomization>));

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TSet<FName> ExcludedNames;
	AssetRegistry.GetDerivedClassNames(TArray<FName>({FName("ModioUserWidgetBase"), FName("Widget")}), ExcludedNames,
									   DerivedClassNames);
	DerivedClassNames.Remove(FName("Widget"));
	DerivedClassNames.Remove(FName("UserWidget"));

	UPackage* ModioUIPackage = FindPackage(nullptr, TEXT("/Script/ModioUI"));
	for (auto& Name : DerivedClassNames)
	{
		UClass* DerivedClass = StaticLoadClass(UObject::StaticClass(), ModioUIPackage, *Name.ToString(), nullptr, LOAD_Quiet);
		if (DerivedClass)
		{
			PropertyModule.RegisterCustomClassLayout(
				DerivedClass->GetFName(),
				FOnGetDetailCustomizationInstance::CreateStatic(
					&FModioEditorHelpers::MakeDetailCustomizationInstance<FModioUserWidgetDetailsCustomization>));
		}
	}

	PropertyModule.RegisterCustomClassLayout(
		CustomClassLayoutNames.Add_GetRef(UModioUserWidgetBase::StaticClass()->GetFName()),
		FOnGetDetailCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakeDetailCustomizationInstance<FModioUserWidgetDetailsCustomization>));
	PropertyModule.RegisterCustomClassLayout(
		CustomClassLayoutNames.Add_GetRef(UWidget::StaticClass()->GetFName()),
		FOnGetDetailCustomizationInstance::CreateStatic(
			&FModioEditorHelpers::MakeDetailCustomizationInstance<FModioUserWidgetDetailsCustomization>));

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FModioUIEditor::UnregisterDetailsCustomizations()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		// unregister properties when the module is shutdown
		FPropertyEditorModule& PropertyModule =
			FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		for (FName& PropertyTypeName : CustomPropertyTypeLayoutNames)
		{
			PropertyModule.UnregisterCustomPropertyTypeLayout(PropertyTypeName);
		}
		for (FName& ClassName : CustomClassLayoutNames)
		{
			PropertyModule.UnregisterCustomClassLayout(ClassName);
		}
		for (FName& ClassName : DerivedClassNames)
		{
			PropertyModule.UnregisterCustomClassLayout(ClassName);
		}
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

void FModioUIEditor::RegisterAssetActions()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetActions = MakeShared<FModioModInfoUIAssetActions>();
	AssetTools.RegisterAssetTypeActions(StaticCastSharedRef<IAssetTypeActions>(AssetActions.ToSharedRef()));
	DialogInfoActions = MakeShared<FAssetTypeActions_ModioDialogInfo>();
	AssetTools.RegisterAssetTypeActions(StaticCastSharedRef<IAssetTypeActions>(DialogInfoActions.ToSharedRef()));
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	ContentBrowserModule.GetAllAssetViewContextMenuExtenders().Add(
		FContentBrowserMenuExtender_SelectedAssets::CreateStatic(
			&FModioUIContentBrowserExtensions::OnExtendContentBrowserAssetSelectionMenu));
	ContentBrowserExtensionsDelegateHandle =
		ContentBrowserModule.GetAllAssetViewContextMenuExtenders().Last().GetHandle();
}

void FModioUIEditor::UnregisterAssetActions()
{
	if (AssetActions)
	{
		FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
		if (AssetToolsModule)
		{
			IAssetTools& AssetTools = AssetToolsModule->Get();
			AssetTools.UnregisterAssetTypeActions(StaticCastSharedRef<IAssetTypeActions>(AssetActions.ToSharedRef()));
			AssetTools.UnregisterAssetTypeActions(
				StaticCastSharedRef<IAssetTypeActions>(DialogInfoActions.ToSharedRef()));
		}
	}
	FContentBrowserModule* ContentBrowserModule = FModuleManager::GetModulePtr<FContentBrowserModule>("ContentBrowser");
	if (ContentBrowserModule)
	{
		ContentBrowserModule->GetAllAssetViewContextMenuExtenders().RemoveAll(
			[this](const FContentBrowserMenuExtender_SelectedAssets& Delegate) {
				return Delegate.GetHandle() == ContentBrowserExtensionsDelegateHandle;
			});
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUIEditor, ModioUIEditor)