#pragma once

#include "Misc/EnumClassFlags.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "UObject/Interface.h"
#include "ModioUISubsystem.h"

#include "IModioUIMediaDownloadCompletedReceiver.generated.h"

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIMediaDownloadEventType : uint8
{
	ModGalleryImages,
	ModCreatorAvatarImage,
	ModLogo
};

ENUM_CLASS_FLAGS(EModioUIMediaDownloadEventType);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIMediaDownloadCompletedReceiver : public UInterface
{
	GENERATED_BODY()
};


//TODO: @modio-ue4 make sure this interface is consistent with that used in other interfaces regarding how the native methods interact with the blueprintimplementable ones etc

class MODIOUI_API IModioUIMediaDownloadCompletedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedMediaDownloadCompleted = false;

	void GalleryImageDownloadHandler(FModioModID ModID, FModioErrorCode ec, int32 Index,
									 TOptional<FModioImageWrapper> Image);
	void ModLogoDownloadHandler(FModioModID ModID, FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

	void CreatorAvatarDownloadHandler(FModioModID ModID, FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

protected:
	template<typename ImplementingClass>
	void Register(EModioUIMediaDownloadEventType DownloadTypes)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModGalleryImages))
			{
				Subsystem->OnModGalleryImageDownloadCompleted.AddUObject(
					Cast<ImplementingClass>(this),
					&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler);
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModLogo))
			{
				Subsystem->OnModLogoDownloadCompleted.AddUObject(
					Cast<ImplementingClass>(this),
					&IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandler);
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCreatorAvatarImage))
			{
				Subsystem->OnModCreatorAvatarDownloadCompleted.AddUObject(
					Cast<ImplementingClass>(this),
					&IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandler);
			}
		}
	}

	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
												  TOptional<FModioImageWrapper> Image);
	virtual void NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ec, int32 ImageIndex,
														  TOptional<FModioImageWrapper> Image);
	virtual void NativeOnModCreatorAvatarDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
														   TOptional<FModioImageWrapper> Image);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ec, int32 ImageIndex,
											FModioOptionalImage Image);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec, FModioOptionalImage Image);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModCreatorAvatarDownloadCompleted(FModioModID ModID, FModioErrorCode ec, FModioOptionalImage Image);
};
