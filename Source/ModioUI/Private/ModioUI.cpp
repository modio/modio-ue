#include "ModioUI.h"

DEFINE_LOG_CATEGORY(ModioUI);

#define LOCTEXT_NAMESPACE "FModioUI"

void FModioUI::StartupModule()
{
	UE_LOG(ModioUI, Display, TEXT("ModioUI5 module has been loaded"));
}

void FModioUI::ShutdownModule()
{
	UE_LOG(ModioUI, Display, TEXT("ModioUI5 module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUI, ModioUI)