#pragma once
#include "CoreMinimal.h"

UENUM()
enum class EMenuAction : uint8
{
	CloseDownloadDrawer,
	CloseSearchDrawer,
	ShowModReportDialog,
	ShowReportEmailDialog,
	ShowUninstallConfirmDialog,
	RefreshDownloadQueue,
	ShowUserAuthenticationDialog,
	ShowUnsubscribeDialog,
	ShowLogoutDialog,

};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMenuAction, EMenuAction, UObject*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCursorVisibilityChanged, bool);
