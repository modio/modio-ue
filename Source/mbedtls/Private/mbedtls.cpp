#include "mbedtls.h"

DEFINE_LOG_CATEGORY(mbedtls);

#define LOCTEXT_NAMESPACE "Fmbedtls"

void Fmbedtls::StartupModule()
{
	UE_LOG(mbedtls, Warning, TEXT("mbedtls module has been loaded"));
}

void Fmbedtls::ShutdownModule()
{
	UE_LOG(mbedtls, Warning, TEXT("mbedtls module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(Fmbedtls, mbedtls)