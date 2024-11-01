/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "PropertyEditorModule.h"

const FName PROPERTY_EDITOR("PropertyEditor");

/**
* @brief A manager class used to register classes for detail customization, to create a window client and to show folders and files selection dialog.
*/
class WindowManager
{

public:

	static WindowManager& Get()
	{
		static WindowManager INSTANCE;
		return INSTANCE;
	}

private:
	WindowManager() {}

public:
	WindowManager(WindowManager const&) = delete;
	void operator=(WindowManager const&) = delete;

private:

	/** @brief Stored property to a window client. */
	TSharedPtr<class SWindow> Window;

public:

	/** @brief A main widget to draw mod creation tool for uploading and editing mods. */
	TSharedPtr<class SModioEditorWindowCompoundWidget> RootWidget;

	/** @brief Creates a new window or returns an existing window. */
	TSharedPtr<class SWindow> GetWindow();

	/** @brief Gets the property editor module for detail customization. */
	FPropertyEditorModule& GetPropertyModule();

	/** @brief Opens a folder dialog for directory selection. */
	void OpenFolderDialog(const FString& Title, FString& OutFolder);

	/** @brief Opens a file dialog for file(s) selection. */
	void OpenFileDialog(const FString& Title, const FString& Filters, bool AllowMultipleSelection, TArray<FString>& OutFiles);

	/** @brief Destroys the main widget and closes the window client. */
	void CloseWindow();

private:

	/** @brief Stored property to registered classes for detail customization. */
	TSet<FName> RegisteredClassNames;

	/** @brief Adds a class in RegisterClassNames for detail customization. */
	void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);

public:

	/** @brief Registers all classes in RegisterClassNames stored property for detail customization. */
	void RegisterObjectCustomizations();

	/** @brief Unregisters all classes in RegisterClassNames stored property from detail customization. */
	void UnregisterObjectCustomizations();
};