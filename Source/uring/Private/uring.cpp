#include "uring.h"

DEFINE_LOG_CATEGORY(uring);

#define LOCTEXT_NAMESPACE "Furing"

void Furing::StartupModule()
{
	UE_LOG(uring, Warning, TEXT("uring module has been loaded"));
}

void Furing::ShutdownModule()
{
	UE_LOG(uring, Warning, TEXT("uring module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(Furing, uring)