#pragma once
#include "Types/ModioAuthenticationParams.h"
#include "UObject/Interface.h"

#include "IModioUIAuthenticationDataProvider.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIAuthenticationProviderInfo
{
	GENERATED_BODY()

	static FModioUIAuthenticationProviderInfo EmailAuth();

	/// @brief Dirty hack to allow encapsulating email auth alongside the true external auth methods
	/// Not exposed to blueprint because it should only be created if the data provider indicates email auth should be
	/// offered
	UPROPERTY()
	bool bIsEmailAuthentication = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!bIsEmailAuthentication", EditConditionHides),
			  Category = "ModioUIAuthenticationProviderInfo")
	EModioAuthenticationProvider ProviderID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioUIAuthenticationProviderInfo")
	FText ProviderUILabel;
};

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIAuthenticationDataProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIAuthenticationDataProvider : public IInterface
{
	GENERATED_BODY()
protected:
	virtual TArray<FModioUIAuthenticationProviderInfo> NativeGetAuthenticationTypes()
		PURE_VIRTUAL(IModioUIAuthenticationDataProvider::NativeGetAuthenticationTypes, return {};);

	virtual FModioAuthenticationParams NativeGetAuthenticationParams(EModioAuthenticationProvider)
		PURE_VIRTUAL(IModioUIAuthenticationDataProvider::NativeGetAuthenticationParams, return {};);

	virtual bool NativeShouldOfferEmailAuthentication()
	{
		return false;
	}

public:
	/// @brief Allows the consuming title to inform the UI which types of external authentication the UI should offer to
	/// the user
	/// @returns An array of EModioAuthenticationProvider values, one for each external authentication to support
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAuthenticationDataProvider")
	TArray<FModioUIAuthenticationProviderInfo> GetAuthenticationTypes();

	TArray<FModioUIAuthenticationProviderInfo> GetAuthenticationTypes_Implementation()
	{
		return NativeGetAuthenticationTypes();
	}

	/// @brief Callback so the consuming application can populate the relevant OAuth token
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAuthenticationDataProvider")
	FModioAuthenticationParams GetAuthenticationParams(EModioAuthenticationProvider Provider);

	FModioAuthenticationParams GetAuthenticationParams_Implementation(EModioAuthenticationProvider Provider)
	{
		return NativeGetAuthenticationParams(Provider);
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAuthenticationDataProvider")
	bool ShouldOfferEmailAuthentication();

	bool ShouldOfferEmailAuthentication_Implementation()
	{
		return NativeShouldOfferEmailAuthentication();
	}
};