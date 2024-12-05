/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "WindowManager.h"
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "DesktopPlatform/Public/IDesktopPlatform.h"
	#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#else
	#include "DesktopPlatformModule.h"
	#include "IDesktopPlatform.h"
#endif
#include "DetailCustomizations/ModioCreateModParamsDetails.h"
#include "DetailCustomizations/ModioBrowseModFileDetails.h"
#include "DetailCustomizations/ModioBrowseModsParamsDetails.h"
#include "DetailCustomizations/ModioCreateModFileParamsDetails.h"
#include "DetailCustomizations/ModioEditModParamsDetails.h"
#include "Objects/ModioCreateModParamsObject.h"
#include "Widgets/SWindow.h"
#include "Widgets/SModioEditorWindowCompoundWidget.h"

TSharedPtr<SWindow> WindowManager::GetWindow()
{
	if (!Window.IsValid()) 
	{
		Window = SNew(SWindow)
		.Title(FText::FromString("mod.io - Content Creation & Upload Tool"))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.HasCloseButton(true)
		.ClientSize(FVector2D(900.f, 600.f))
		.SizingRule(ESizingRule::FixedSize)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.ScreenPosition(FVector2D(0, 0))
		.LayoutBorder(FMargin(3.f))
		[
			SAssignNew(RootWidget, SModioEditorWindowCompoundWidget)
		];
		Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& WindowRef) 
		{
			RootWidget->UnloadResources();
			RootWidget = nullptr;
			Window = nullptr;
		}));
		FSlateApplication::Get().AddWindow(Window.ToSharedRef(), false);
	}
	Window->BringToFront(true);
	return Window;
}

FPropertyEditorModule& WindowManager::GetPropertyModule()
{
	return FModuleManager::GetModuleChecked<FPropertyEditorModule>(PROPERTY_EDITOR);
}

void WindowManager::OpenFolderDialog(const FString& Title, FString& OutFolder) 
{
	if (Window.IsValid())
	{
		if (Window->GetNativeWindow().IsValid())
		{
			void* ParentWindowHandle = Window->GetNativeWindow()->GetOSWindowHandle();

			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				FString OutFolderName;
				if(DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, Title, FPaths::ProjectDir(), OutFolderName))
				{
					OutFolder = OutFolderName;
				}
			}
		}
	}
}

void WindowManager::OpenFileDialog(const FString& Title, const FString& Filters, bool AllowMultipleSelection, TArray<FString>& OutFiles) 
{
	if (Window.IsValid())
	{
		if (Window->GetNativeWindow().IsValid())
		{
			void* ParentWindowHandle = Window->GetNativeWindow()->GetOSWindowHandle();

			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				TArray<FString> OutFileName;
				if (DesktopPlatform->OpenFileDialog(ParentWindowHandle, Title, TEXT(""), TEXT(""), Filters, AllowMultipleSelection, OutFileName))
				{
					OutFiles = OutFileName;
				}
			}
		}
	}
}

void WindowManager::CloseWindow() 
{	
	if (Window.IsValid())
	{
		Window->RequestDestroyWindow();
	}
}

void WindowManager::RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
	check(ClassName != NAME_None);
	RegisteredClassNames.Add(ClassName);
	GetPropertyModule().RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}

void WindowManager::RegisterObjectCustomizations() 
{
	RegisterCustomClassLayout("ModioCreateModParamsObject", FOnGetDetailCustomizationInstance::CreateStatic(&ModioCreateModParamsDetails::MakeInstance));
	RegisterCustomClassLayout("ModioBrowseModsObject", FOnGetDetailCustomizationInstance::CreateStatic(&ModioBrowseModsParamsDetails::MakeInstance));
	RegisterCustomClassLayout("ModioEditModParamsObject", FOnGetDetailCustomizationInstance::CreateStatic(&ModioEditModParamsDetails::MakeInstance));
	RegisterCustomClassLayout("ModioBrowseModFileCollectionObject", FOnGetDetailCustomizationInstance::CreateStatic(&ModioBrowseModFileDetails::MakeInstance));
	RegisterCustomClassLayout("ModioCreateNewModFileParamsObject", FOnGetDetailCustomizationInstance::CreateStatic(&ModioCreateModFileParamsDetails::MakeInstance));
}

void WindowManager::UnregisterObjectCustomizations() 
{
	if (FModuleManager::Get().IsModuleLoaded(PROPERTY_EDITOR)) 
	{
		FPropertyEditorModule& PropertyModule = GetPropertyModule();
		for (auto registry = RegisteredClassNames.CreateConstIterator(); registry; ++registry) 
		{
			if (registry->IsValid())
			{
				PropertyModule.UnregisterCustomClassLayout(*registry);
			}
		}
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}