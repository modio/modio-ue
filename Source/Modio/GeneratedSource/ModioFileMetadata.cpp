/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io SDK.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *
 */

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/core/entities/ModioFileMetadata.h"
#else
	#pragma once
#endif

#include "modio/detail/ModioJsonHelpers.h"
#include "nlohmann/json.hpp"

namespace Modio
{
	NLOHMANN_JSON_SERIALIZE_ENUM(FileMetadata::VirusScanStatus, {{FileMetadata::VirusScanStatus::NotScanned, 0},
																 {FileMetadata::VirusScanStatus::ScanComplete, 1},
																 {FileMetadata::VirusScanStatus::InProgress, 2},
																 {FileMetadata::VirusScanStatus::TooLargeToScan, 3},
																 {FileMetadata::VirusScanStatus::FileNotFound, 4},
																 {FileMetadata::VirusScanStatus::ErrorScanning, 5}})

	NLOHMANN_JSON_SERIALIZE_ENUM(FileMetadata::VirusStatus,
								 {{FileMetadata::VirusStatus::NoThreat, 0}, {FileMetadata::VirusStatus::Malicious, 1}})

	void from_json(const nlohmann::json& Json, Modio::FileMetadata& FileMetadata)
	{
		std::string debug = Json.dump();
		Detail::ParseSafe(Json, FileMetadata.MetadataId, "id");
		Detail::ParseSafe(Json, FileMetadata.ModId, "mod_id");
		Detail::ParseSafe(Json, FileMetadata.DateAdded, "date_added");
		Detail::ParseSafe(Json, FileMetadata.CurrentVirusScanStatus, "virus_status");
		Detail::ParseSafe(Json, FileMetadata.CurrentVirusStatus, "virus_positive");
		Detail::ParseSafe(Json, FileMetadata.Filesize, "filesize");
		Detail::ParseSafe(Json, FileMetadata.Filename, "filename");
		Detail::ParseSafe(Json, FileMetadata.Version, "version");
		Detail::ParseSafe(Json, FileMetadata.Changelog, "changelog");
		Detail::ParseSafe(Json, FileMetadata.MetadataBlob, "metadata_blob");
		Detail::ParseSubobjectSafe(Json, FileMetadata.DownloadBinaryURL, "download", "binary_url");
		Detail::ParseSubobjectSafe(Json, FileMetadata.DownloadExpiryDate, "download", "date_expires");
	}

	void to_json(nlohmann::json& Json, const Modio::FileMetadata& FileMetadata)
	{
		Json = {{"id", FileMetadata.MetadataId},
				{"mod_id", FileMetadata.ModId},
				{"date_added", FileMetadata.DateAdded},
				{"virus_status", FileMetadata.CurrentVirusScanStatus},
				{"virus_positive", FileMetadata.CurrentVirusStatus},
				{"filesize", FileMetadata.Filesize},
				{"filename", FileMetadata.Filename},
				{"version", FileMetadata.Version},
				{"changelog", FileMetadata.Changelog},
				{"metadata_blob", FileMetadata.MetadataBlob},
				{"download", nlohmann::json::object({{"binary_url", FileMetadata.DownloadBinaryURL},
													 {"date_expires", FileMetadata.DownloadExpiryDate}})}};
	}
} // namespace Modio
