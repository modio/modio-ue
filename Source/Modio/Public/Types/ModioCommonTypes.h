/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Misc/Crc.h"

#include "ModioCommonTypes.generated.h"

// Forward decls
namespace Modio
{
	struct ModID;
	struct GameID;
	struct FileMetadataID;
	struct UserID;
} // namespace Modio

/** @brief Enum representing what environment your game is deployed in */
UENUM(BlueprintType)
enum class EModioEnvironment : uint8
{
	// Test/Private environment
	Test,
	// Live/Public environment
	Live
};

/** @brief Enum representing the store or service your game is being distributed through */
UENUM(BlueprintType)
enum class EModioPortal : uint8
{
	None,
	Apple,
	EpicGamesStore,
	GOG,
	Google,
	Itchio,
	Nintendo,
	PSN,
	Steam,
	XboxLive
};

/** @brief Enum representing mod logo sizes */
UENUM(BlueprintType)
enum class EModioLogoSize : uint8
{
	// Original Size
	Original,
	// 320x180px
	Thumb320,
	// 640x360px
	Thumb640,
	// 1280x720px
	Thumb1280
};

/** @brief Enum representing avatar image sizes */
UENUM(BlueprintType)
enum class EModioAvatarSize : uint8
{
	// Original Size
	Original,
	// 50x50px Thumbnail
	Thumb50,
	// 100x100px Thumbnail
	Thumb100
};

/** */
UENUM(BlueprintType)
enum class EModioGallerySize : uint8
{
	// Original Size
	Original,
	// 320x180px Thumbnail
	Thumb320
};

/** @brief Degree of severity for the log output */
UENUM(BlueprintType)
enum class EModioLogLevel : uint8
{
	// Detailed low-level debugging output. Not intended for general use
	Trace = 0,
	// Informational output containing status messages
	Info = 1,
	// Warnings about incorrect plugin usage, timeouts
	Warning = 2,
	// Only errors
	Error = 3
};

/** @brief Enum representing the languages mod.io support responses in */
UENUM(BlueprintType)
enum class EModioLanguage : uint8
{
	English,
	Bulgarian,
	French,
	German,
	Italian,
	Polish,
	Portuguese,
	Hungarian,
	Japanese,
	Korean,
	Russian,
	Spanish,
	Thai,
	ChineseSimplified,
	ChineseTraditional
};

/** @brief Strong type for Mod IDs */
USTRUCT(BlueprintType)
struct MODIO_API FModioModID
{
	GENERATED_BODY()

	FModioModID();
	constexpr explicit FModioModID(int64 InModId) : ModID(InModId) {}

	MODIO_API friend uint32 GetTypeHash(FModioModID ModioModId);

	MODIO_API friend bool operator==(FModioModID A, FModioModID B)
	{
		return A.ModID == B.ModID;
	}

	MODIO_API friend bool operator!=(FModioModID A, FModioModID B)
	{
		return A.ModID != B.ModID;
	}

	friend bool operator<(FModioModID A, FModioModID B)
	{
		return A.ModID < B.ModID;
	}

	friend bool operator>(FModioModID A, FModioModID B)
	{
		return A.ModID > B.ModID;
	}

	FString ToString() const
	{
		if (ModID < 0)
		{
			return TEXT("InvalidModID");
		}
		return FString::Printf(TEXT("%lld"), ModID);
	}

	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioModID& ID)
	{
		return Ar << ID.ModID;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	friend struct Modio::ModID ToModio(const FModioModID& In);
	int64 ModID;
};

template<>
struct TStructOpsTypeTraits<FModioModID> : public TStructOpsTypeTraitsBase2<FModioModID>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithNetSerializer = true
	};
};

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModID
{
	GENERATED_BODY()

	TOptional<FModioModID> Internal;
};

/** @brief Strong type for Game IDs */
USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeGameId"))
struct MODIO_API FModioGameID
{
	GENERATED_BODY()

	FModioGameID();
	constexpr explicit FModioGameID(int64 InGameId) : GameId(InGameId) {}

	MODIO_API friend uint32 GetTypeHash(FModioGameID ModioGameId);

	MODIO_API friend bool operator==(FModioGameID A, FModioGameID B)
	{
		return A.GameId == B.GameId;
	}

	FString ToString() const
	{
		if (GameId < 0)
		{
			return TEXT("InvalidGameID");
		}
		return FString::Printf(TEXT("%lld"), GameId);
	}

	static FModioGameID InvalidGameID();

	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioGameID& ID)
	{
		return Ar << ID.GameId;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	friend struct FModioGameIDStructCustomization;
	friend struct Modio::GameID ToModio(const FModioGameID& In);
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	int64 GameId;

};

template<>
struct TStructOpsTypeTraits<FModioGameID> : public TStructOpsTypeTraitsBase2<FModioGameID>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithNetSerializer = true
	};
};

/** @brief Strong type for File Metadata IDs */
USTRUCT(BlueprintType)
struct MODIO_API FModioFileMetadataID
{
	GENERATED_BODY()

	FModioFileMetadataID();
	constexpr explicit FModioFileMetadataID(int64 InFileMetadataId) : FileMetadataID(InFileMetadataId) {}

	MODIO_API friend uint32 GetTypeHash(FModioFileMetadataID FileMetadataID);

	MODIO_API friend bool operator==(FModioFileMetadataID A, FModioFileMetadataID B)
	{
		return A.FileMetadataID == B.FileMetadataID;
	}

	FString ToString() const
	{
		if (FileMetadataID < 0)
		{
			return TEXT("InvalidFileMetadataID");
		}
		return FString::Printf(TEXT("%lld"), FileMetadataID);
	}

	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioFileMetadataID& ID)
	{
		return Ar << ID.FileMetadataID;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	friend struct Modio::FileMetadataID ToModio(const FModioFileMetadataID& In);
	int64 FileMetadataID;
};

template<>
struct TStructOpsTypeTraits<FModioFileMetadataID> : public TStructOpsTypeTraitsBase2<FModioFileMetadataID>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithNetSerializer = true
	};
};

/** @brief Strong type for User IDs */
USTRUCT(BlueprintType)
struct MODIO_API FModioUserID
{
	GENERATED_BODY()

	FModioUserID();

	constexpr explicit FModioUserID(int64 InUserID) : UserID(InUserID) {}

	MODIO_API friend uint32 GetTypeHash(FModioUserID UserID);

	MODIO_API friend bool operator==(FModioUserID A, FModioUserID B)
	{
		return A.UserID == B.UserID;
	}

	FString ToString() const
	{
		if (UserID < 0)
		{
			return TEXT("InvalidUserID");
		}
		return FString::Printf(TEXT("%lld"), UserID);
	}

	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioUserID& ID)
	{
		return Ar << ID.UserID;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	friend struct Modio::UserID ToModio(const FModioUserID& In);
	int64 UserID;
};

template<>
struct TStructOpsTypeTraits<FModioUserID> : public TStructOpsTypeTraitsBase2<FModioUserID>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithNetSerializer = true
	};
};

/** @brief Strong type for Api Keys */
USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeApiKey"))
struct MODIO_API FModioApiKey
{
	GENERATED_BODY()

	FModioApiKey() = default;
	explicit FModioApiKey(const FString& InApiKey);

	const FString& ToString() const
	{
		// Put in the function instead of default constructor to avoid having to allocate memory for
		// each empty instance
		if (ApiKey.Len() == 0)
		{
			static FString Invalid(TEXT("InvalidApiKey"));
			return Invalid;
		}
		return ApiKey;
	}

	static FModioApiKey InvalidAPIKey();

private:
	friend struct FModioApiKeyStructCustomization;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	FString ApiKey;
};

/** @brief Strong type for email address */
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAddress
{
	GENERATED_BODY()

	FModioEmailAddress() = default;
	FModioEmailAddress(const FString& InEmailAddress);

	const FString& ToString() const
	{
		// Put in the function instead of default constructor to avoid having to allocate memory for
		// each empty instance
		if (EmailAddress.Len() == 0)
		{
			static FString Invalid(TEXT("InvalidEmailAddress"));
			return Invalid;
		}
		return EmailAddress;
	}

private:
	FString EmailAddress;
};

/** @brief Strong type for email auth code */
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAuthCode
{
	GENERATED_BODY()

	FModioEmailAuthCode() = default;
	FModioEmailAuthCode(const FString& InEmailAuthCode);

	const FString& ToString() const
	{
		// Put in the function instead of default constructor to avoid having to allocate memory for
		// each empty instance
		if (EmailAuthCode.Len() == 0)
		{
			static FString Invalid(TEXT("InvalidEmailAuthCode"));
			return Invalid;
		}
		return EmailAuthCode;
	}

private:
	FString EmailAuthCode;
};

#pragma endregion
