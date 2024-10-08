// Copyright Blackbird Interactive. All rights reserved.

#include "ModioUpdateStaticLocDataCommandlet.h"

#if WITH_EDITOR

	#include "AssetRegistry/AssetRegistryModule.h"
	#include "Internationalization/StringTable.h"
	#include "Internationalization/StringTableCore.h"
	#include "FileHelpers.h"
	#include "Loc/ModioEnumLocalizationHelpers.h"
	#include "ModioErrorCondition.h"
	#include "ModioSettings.h"
	#include "Types/ModioFilterParams.h"
	#include "Types/ModioModCollectionEntry.h"
	#include "Types/ModioModManagementEvent.h"
	#include "Types/ModioModProgressInfo.h"
	#include "Types/ModioReportParams.h"

DEFINE_LOG_CATEGORY_STATIC(LogModioUpdateStaticLocDataCommandlet, All, All);

UModioUpdateStaticLocDataCommandlet::UModioUpdateStaticLocDataCommandlet()
{
	IsClient = false;
	IsEditor = true;
	IsServer = false;
	LogToConsole = true;

	HelpDescription = TEXT("Commandlet updating static localization data for the mod.io plugin.");
}

int UModioUpdateStaticLocDataCommandlet::Main(const FString& Params)
{
	// Parse command line
	TArray<FString> Tokens;
	TArray<FString> Switches;
	TMap<FString, FString> ParamVals;
	UCommandlet::ParseCommandLine(*Params, Tokens, Switches, ParamVals);
	if (const UModioSettings* PluginSettings = GetDefault<UModioSettings>())
	{
		if (UObject* MaybeStringTable = PluginSettings->LocalizationStringTable.TryLoad())
		{
			UStringTable* StringTableObject = Cast<UStringTable>(MaybeStringTable);
			if (!StringTableObject)
			{
				UE_LOG(LogModioUpdateStaticLocDataCommandlet, Display,
					   TEXT("Could not resolve string table for static localization data. Is "
							"UModioSettings::LocalizationStringTable configured correctly?"));
				return -1;
			}
			FStringTableRef StaticLocDataTable = StringTableObject->GetMutableStringTable();

			ModioUI::RegisterEnumAsLocalizable<EModioReportType>(StaticLocDataTable);
			ModioUI::RegisterEnumAsLocalizable<EModioSortFieldType>(StaticLocDataTable);
			ModioUI::RegisterEnumAsLocalizable<EModioSortDirection>(StaticLocDataTable);
			ModioUI::RegisterEnumAsLocalizable<EModioModProgressState>(StaticLocDataTable);
			ModioUI::RegisterEnumAsLocalizable<EModioModState>(StaticLocDataTable);
			ModioUI::RegisterEnumAsLocalizable<EModioModManagementEventType>(StaticLocDataTable);
			ModioUI::RegisterEnumAsLocalizable<EFileSizeUnit>(StaticLocDataTable);

			// Localize Error Conditions so they can be displayed to end-users in the UI
			// Error conditions are code-generated by the SDK so they need manual display string overrides

			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::NoError, "No Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::NetworkError,
														 "Network Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::ConfigurationError,
														 "Configuration Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::InvalidArgsError,
														 "Invalid Parameter");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::FilesystemError,
														 "Filesystem Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::InternalError,
														 "System Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::ApiErrorRefSuccess,
														 "No Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::EntityNotFoundError,
														 "Item not found");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::UserTermsOfUseError,
														 "Terms of Use not accepted");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::SubmitReportError,
														 "Report submission error");
			ModioUI::RegisterCustomEnumValueLocalization(
				StaticLocDataTable, EModioErrorCondition::UserNotAuthenticatedError, "Authentication failure");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::SystemError,
														 "System Error");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::OperationCanceled,
														 "Canceled");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::RateLimited,
														 "Rate Limit hit");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::ModBeingProcessed,
														 "Mod is being processed");
			ModioUI::RegisterCustomEnumValueLocalization(StaticLocDataTable, EModioErrorCondition::InsufficientSpace,
														 "Insufficient space");
			ModioUI::RegisterCustomEnumValueLocalization(
				StaticLocDataTable, EModioErrorCondition::InsufficientPermissions, "Access denied");

			if (UEditorLoadingAndSavingUtils::SavePackages({StringTableObject->GetPackage()},
														   false))
			{
				UE_LOG(LogModioUpdateStaticLocDataCommandlet, Display,
					   TEXT("String table updated successfully with static localization data."));
			}
		}
	}
	return 0;
}

#endif
