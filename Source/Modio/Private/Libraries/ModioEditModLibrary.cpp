/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioEditModLibrary.h"

void UModioEditModLibrary::SetName(FModioEditModParams& In, FString Name)
{
	In.Name = Name;
}

void UModioEditModLibrary::SetSummary(FModioEditModParams& In, FString Summary)
{
	In.Summary = Summary;
}

void UModioEditModLibrary::SetNamePath(FModioEditModParams& In, FString NamePath)
{
	In.NamePath = NamePath;
}

void UModioEditModLibrary::SetVisibility(FModioEditModParams& In, bool Visibility)
{
	In.bVisible = Visibility;
}

void UModioEditModLibrary::SetDescription(FModioEditModParams& In, FString Description)
{
	In.Description = Description;
}

void UModioEditModLibrary::SetHomepageURL(FModioEditModParams& In, FString HomepageURL)
{
	In.HomepageURL = HomepageURL;
}

void UModioEditModLibrary::SetMaturityFlags(FModioEditModParams& In, EModioMaturityFlags MaturityFlags)
{
	In.MaturityFlags = MaturityFlags;
}

void UModioEditModLibrary::SetMetadataBlob(FModioEditModParams& In, FString MetadataBlob)
{
	In.MetadataBlob = MetadataBlob;
}
