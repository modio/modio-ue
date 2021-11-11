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
	#include "modio/detail/compression/zip/ArchiveFileImplementation.h"
#endif

namespace Modio
{
	namespace Detail
	{
		void ArchiveFileImplementation::AddEntry(ArchiveEntry Entry)
		{
			ArchiveEntries.push_back(Entry);
		}

		void ArchiveFileImplementation::AddEntry(std::string FileName, std::uintmax_t FileOffset,
												 std::uintmax_t CompressedSize, std::uintmax_t UncompressedSize,
												 CompressionMethod Compression, std::uint32_t CRCValue, bool bIsDirectory)
		{
			ArchiveEntries.push_back(
				ArchiveEntry {Compression, FileName, FileOffset, CompressedSize, UncompressedSize, CRCValue, bIsDirectory});
		}

		std::uintmax_t ArchiveFileImplementation::GetNumberOfEntries()
		{
			return ArchiveEntries.size();
		}

		std::vector<ArchiveFileImplementation::ArchiveEntry>::iterator ArchiveFileImplementation::begin()
		{
			return ArchiveEntries.begin();
		}

		std::vector<ArchiveFileImplementation::ArchiveEntry>::iterator ArchiveFileImplementation::end()
		{
			return ArchiveEntries.end();
		}

	} // namespace Detail
} // namespace Modio
