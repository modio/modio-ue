#pragma once

#include "Containers/Array.h"
#include "Types/ModioAuthenticationParams.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"
#include "UObject/Object.h"

#include "ModioUIDefaultAuthProvider.generated.h"

UCLASS()
class MODIOUI_API UModioUIDefaultAuthProvider : public UObject, public IModioUIAuthenticationDataProvider
{
	GENERATED_BODY()
protected:
	TArray<FModioUIAuthenticationProviderInfo> NativeGetAuthenticationTypes() override
	{
		// Default implementation, the built-in default provider only supports email auth
		return {};
	}

	FModioAuthenticationParams NativeGetAuthenticationParams(EModioAuthenticationProvider) override
	{
		// Default implementation, the built-in default provider only supports email auth
		return {};
	}

	bool NativeShouldOfferEmailAuthentication() override
	{
		return true;
	}
};