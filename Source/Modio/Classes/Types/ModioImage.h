#pragma once

#include "Containers/UnrealString.h"
#include "ModioSDK.h"
#include "Types/ModioImageState.h"

// clang-format off
#include "ModioImage.generated.h"
// clang-format on

DECLARE_DELEGATE_OneParam(FOnLoadImageDelegateFast, class UTexture2DDynamic*);

/** @brief wrapper for images that's returned by the api */
USTRUCT(BlueprintType)
struct MODIO_API FModioImage
{
	GENERATED_BODY()

	FModioImage() = default;
	FModioImage(const Modio::filesystem::path& Path);

	/** Get the texture if if has been loaded by any FModioImage instance */
	class UTexture2DDynamic* GetTexture() const;

	/** Get the current state of the image */
	EModioImageState GetState() const;

	/** Load the texture into memory async, if it's already in memory, then we will return immediately */
	void LoadAsync(FOnLoadImageDelegateFast Callback) const;

	/** The path on disc to the image */
	UPROPERTY(BlueprintReadOnly, Category = "Media")
	FString ImagePath;
private:
	// Data required to create the texture
	struct FTextureCreationData
	{
		TArray<uint8> RawData;
		int32 Width;
		int32 Height;
	};

	/** Internal loader that will load the texture sync */
	static TOptional<FTextureCreationData> LoadTextureDataFromDisk(const FString& ImagePath);
};

USTRUCT(BlueprintType)
struct FModioOptionalImage
{
	GENERATED_BODY()

	// This is here due to it was needed once, but now I get crashes if I remove it
	bool Serialize(FArchive& Ar)
	{
		return true;
	}

	TOptional<FModioImage> Internal;
};

// This is here due to it was needed once, but now I get crashes if I remove it
template<>
struct TStructOpsTypeTraits<FModioOptionalImage> : public TStructOpsTypeTraitsBase2<FModioOptionalImage>
{
	enum
	{
		WithSerializer = true
	};
};
