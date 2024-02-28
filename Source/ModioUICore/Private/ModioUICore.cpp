#include "ModioUICore.h"

DEFINE_LOG_CATEGORY(ModioUICore);

#define LOCTEXT_NAMESPACE "FModioUICore"

void FModioUICore::StartupModule()
{
	UE_LOG(ModioUICore, Display, TEXT("ModioUICore module has been loaded"));
}

void FModioUICore::ShutdownModule()
{
	UE_LOG(ModioUICore, Display, TEXT("ModioUICore module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUICore, ModioUICore)