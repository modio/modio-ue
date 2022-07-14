#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"

void IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler(FModioModID ModID, FModioErrorCode ec,
																		 int32 Index,
																		 TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModGalleryImageDownloadCompleted(ModID, ec, Index, Image);
	checkf(bRoutedMediaDownloadCompleted,
		   TEXT("NativeOnModGalleryImageDownloadCompleted should call Super::NativeOnModGalleryImageDownloadCompleted "
				"to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandler(FModioModID ModID, FModioErrorCode ec,
																	TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModLogoDownloadCompleted(ModID, ec, Image);
	checkf(bRoutedMediaDownloadCompleted, TEXT("NativeOnModLogoDownloadCompleted should call "
											   "Super::NativeOnModLogoDownloadCompleted to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandler(FModioModID ModID, FModioErrorCode ec,
																		  TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModCreatorAvatarDownloadCompleted(ModID, ec, Image);
	checkf(bRoutedMediaDownloadCompleted,
		   TEXT("NativeOnModCreatorAvatarDownloadCompleted should call "
				"Super::NativeOnModCreatorAvatarDownloadCompleted to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																			  TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModLogoDownloadCompleted(Cast<UObject>(this), ModID, ec, FModioOptionalImage {Image});
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModGalleryImageDownloadCompleted(
	FModioModID ModID, FModioErrorCode ec, int32 ImageIndex, TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModGalleryImageDownloadCompleted(Cast<UObject>(this), ModID, ec, ImageIndex, FModioOptionalImage {Image});
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModCreatorAvatarDownloadCompleted(
	FModioModID ModID, FModioErrorCode ec, TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModCreatorAvatarDownloadCompleted(Cast<UObject>(this), ModID, ec, FModioOptionalImage {Image});
}
