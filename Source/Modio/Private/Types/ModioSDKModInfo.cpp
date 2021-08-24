#include "Types/ModioModInfo.h"
#include "Internal/ModioConvert.h"

FModioModInfo::FModioModInfo(const Modio::ModInfo& ModInfo)
	: ModId(ToUnreal(ModInfo.ModId)),
	  ProfileName(ToUnreal(ModInfo.ProfileName)),
	  ProfileSummary(ToUnreal(ModInfo.ProfileSummary)),
	  ProfileDescription(ToUnreal(ModInfo.ProfileDescription)),
	  ProfileDescriptionPlaintext(ToUnreal(ModInfo.ProfileDescriptionPlaintext)),
	  ProfileURL(ToUnreal(ModInfo.ProfileURL)),
	  ProfileSubmittedBy(ToUnreal(ModInfo.ProfileSubmittedBy)),
	  ProfileDateAdded(ToUnrealDateTime(ModInfo.ProfileDateAdded)),
	  ProfileDateUpdated(ToUnrealDateTime(ModInfo.ProfileDateUpdated)),
	  ProfileDateLive(ToUnrealDateTime(ModInfo.ProfileDateLive)),
	  ProfileMaturityOption(ToUnreal(ModInfo.ProfileMaturityOption)),
	  MetadataBlob(ModInfo.MetadataBlob.c_str()), // Don't use ToUnreal as that does UTF8->TCHAR conversion
	  FileInfo(ToUnreal(ModInfo.FileInfo)),
	  MetadataKvp(ToUnreal<FModioMetadata>(ModInfo.MetadataKvp)),
	  Tags(ToUnreal<FModioModTag>(ModInfo.Tags)),
	  NumGalleryImages(ToUnreal(ModInfo.NumGalleryImages)),
	  YoutubeURLs(ToUnreal(ModInfo.YoutubeURLs)),
	  SketchfabURLs(ToUnreal(ModInfo.SketchfabURLs)),
	  Stats(ToUnreal(ModInfo.Stats))
{}
