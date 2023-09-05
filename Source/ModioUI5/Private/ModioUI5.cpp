#include "ModioUI5.h"

DEFINE_LOG_CATEGORY(ModioUI5);

#define LOCTEXT_NAMESPACE "FModioUI5"

void FModioUI5::StartupModule()
{
	UE_LOG(ModioUI5, Warning, TEXT("ModioUI5 module has been loaded"));
}

void FModioUI5::ShutdownModule()
{
	UE_LOG(ModioUI5, Warning, TEXT("ModioUI5 module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUI5, ModioUI5)