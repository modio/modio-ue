#pragma once

#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIUserAvatarDownloadCompletedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIUserAvatarDownloadCompletedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIUserAvatarDownloadCompletedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedAvatarDownloaded = false;
	void UserAvatarDownloadCompletedHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnUserAvatarDownloadCompleted.AddUObject(
				Cast<ImplementingClass>(this),
				&IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandler);
		}
	}

	virtual void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUserAvatarDownloadCompleted(FModioErrorCode ec, FModioOptionalImage Image);
};
