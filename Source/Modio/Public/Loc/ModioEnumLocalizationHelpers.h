#pragma once

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Internationalization/Text.h"
#include "ModioSettings.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUnsigned64.h"
#include "UObject/Class.h"
#include "UObject/NameTypes.h"

#include "ModioEnumLocalizationHelpers.generated.h"

namespace ModioUI
{
	template<typename EnumType>
	void RegisterEnumAsLocalizable(FStringTableRef TargetTable)
	{
		UEnum* EnumReflectionData = StaticEnum<EnumType>();

		int32 NumEnumEntries = EnumReflectionData->NumEnums() - 1; // Don't count the automatically added _MAX entry
		for (int32 CurrentEntryIndex = 0; CurrentEntryIndex < NumEnumEntries; CurrentEntryIndex++)
		{
			FName NameStr = EnumReflectionData->GetNameByIndex(CurrentEntryIndex);

			FText DisplayString = EnumReflectionData->GetDisplayNameTextByIndex(CurrentEntryIndex);
			TargetTable->SetSourceString(NameStr.ToString(), DisplayString.ToString());
		}
	}
	template<typename EnumType>
	void RegisterCustomEnumValueLocalization(FStringTableRef TargetTable, EnumType EnumValue, FString SourceString)
	{
		UEnum* EnumReflectionData = StaticEnum<EnumType>();

		FName EnumEntryNameStr = EnumReflectionData->GetNameByValue(static_cast<int64>(EnumValue));
		TargetTable->SetSourceString(EnumEntryNameStr.ToString(), SourceString);
	}
} // namespace ModioUI

/**
 * @docpublic
 * @brief Utility functions for UI enum localization in mod.io
 */
UCLASS()
class MODIO_API UModioUIEnumLocalizationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// I'd prefer to do this such that you can pass in the enum value rather than the name but don't really want to deal
	// with a whole custom K2Node to properly support generic enum values
	
	/**
	 * @docpublic
	 * @brief Returns the localized string table `FText` for a given enum value's `FName`. 
	 *        Only works with enums registered via `ModioUI::RegisterEnumAsLocalizable`.
	 * @param EnumName The `FName` of the given enum value to fetch its localized string
	 * @return Localized `FText` for the specified enum value, or a dummy FText if not found
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Localization")
	static FText GetLocalizedTextForEnumByName(const FName& EnumName)
	{
		if (const UModioSettings* PluginSettings = GetDefault<UModioSettings>())
		{
			UStringTable* ModioStringTableAsset = Cast<UStringTable>(PluginSettings->LocalizationStringTable.TryLoad());

			if (ModioStringTableAsset)
			{
				auto StaticLocDataTable = ModioStringTableAsset->GetMutableStringTable();
				return FText::FromStringTable(ModioStringTableAsset->GetStringTableId(), EnumName.ToString());
			}
		}
		return FText::FromName(EnumName);
	}

	/**
	 * @docpublic
	 * @brief Converts an Unsigned64 filesize to a human-readable string with the appropriate unit.
	 * 
	 * The function attempts to display the size in the largest unit that will have an integral 
	 * part > 0, or in the specified unit if `Unit` is set to something other than `Largest`.
	 * @param FileSize The filesize in bytes to convert to human-readable format
	 * @param MinDecimals The minimum number of decimals to display for the filesize
	 * @param MaxDecimals The maximum number of decimals to display for the filesize
	 * @param Unit The unit to display the filesize in (defaults to the largest appropriate unit)
	 * @param bIncludeUnitName Whether or not to include the unit name in the result
	 * @return An FText formatted with the specified filesize and unit
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Text",
			  meta = (DisplayName = "FileSizeToText (Unsigned64)", CompactNodeTitle = "FileSizeUnsigned64>Text"))
	static FText FileSizeUnsigned64_ToText(FModioUnsigned64 FileSize, int32 MinDecimals = 0, int32 MaxDecimals = 2,
										   EFileSizeUnit Unit = EFileSizeUnit::Largest, bool bIncludeUnitName = true)
	{
		static const int32 KB = 1024;
		static const int32 MB = 1024 * 1024;
		static const int32 GB = 1024 * 1024 * 1024;

		if (Unit == EFileSizeUnit::Largest)
		{
			Unit = EFileSizeUnit::B;
			if (FileSize.Underlying > KB)
			{
				Unit = EFileSizeUnit::KB;
			}
			if (FileSize.Underlying > MB)
			{
				Unit = EFileSizeUnit::MB;
			}
			if (FileSize.Underlying > GB)
			{
				Unit = EFileSizeUnit::GB;
			}
		}

		double InNewUnit = (double) FileSize / static_cast<double>(Unit);

		FFormatNamedArguments Args;

		FNumberFormattingOptions FormatRules;
		FormatRules.MinimumFractionalDigits = MinDecimals;
		FormatRules.MaximumFractionalDigits = MaxDecimals;
		FormatRules.MinimumIntegralDigits = 1;

		Args.Add(TEXT("FileSize"), FText::AsNumber(InNewUnit, &FormatRules));
		Args.Add(TEXT("UnitName"),
				 bIncludeUnitName ? GetLocalizedTextForEnumByName(StaticEnum<EFileSizeUnit>()->GetNameByValue(Unit))
								  : FText::GetEmpty());

		return FText::Format(FTextFormat::FromString(TEXT("{FileSize}{UnitName}")), Args);
	}
};

/**
 * @docpublic
 * @brief Utility functions for UI localization in mod.io
 */
UCLASS()
class MODIO_API UModioUILocalizationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Returns the localized string table Text for a given string key
	 * @param StringKey The key to look up in the string table
	 * @return Localized FText for the specified key, or StringKey itself if not found
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Localization")
	static FText GetLocalizedTextFromDefaultTableByKey(const FString& StringKey)
	{
		if (const UModioSettings* PluginSettings = GetDefault<UModioSettings>())
		{
			UStringTable* ModioStringTableAsset = Cast<UStringTable>(PluginSettings->LocalizationStringTable.TryLoad());

			if (ModioStringTableAsset)
			{
				auto StaticLocDataTable = ModioStringTableAsset->GetMutableStringTable();
				return FText::FromStringTable(ModioStringTableAsset->GetStringTableId(), StringKey);
			}
		}
		return FText::FromString(StringKey);
	}
};