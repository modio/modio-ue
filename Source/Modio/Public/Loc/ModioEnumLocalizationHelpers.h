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
	};
}; // namespace ModioUI

UCLASS()
class MODIO_API UModioUIEnumLocalizationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// I'd prefer to do this such that you can pass in the enum value rather than the name but don't really want to deal
	// with a whole custom K2Node to properly support generic enum values

	/// @brief Returns the string table FText for a given enum value's FName - *Only works with enums registered via
	/// ModioUI::RegisterEnumAsLocalizable*
	/// @param EnumName The Name from a given enum value
	/// @return localized Text for the specified enum value, or dummy FText if not found
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
	 * Converts a Unsigned64 filesize to a human readable string with the appropriate unit
	 *
	 * @param FileSize - Filesize in bytes
	 * @param MaxDecimals - Maximum amount of decimals to display of the filesize
	 * @param Unit - If Largest, then it tries to display the size in the largest unit that will have a integral
	 * part > 0, else it displays the filesize in the specified unit
	 * @return A text formatted from your specifications
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

UCLASS()
class MODIO_API UModioUILocalizationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/// @brief Returns the string table FText for a given string key
	/// @param StringKey The key to look up in the table
	/// @return localized Text for the specified key, or StringKey if not found
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