#pragma once

#include "Misc/Crc.h"
#include "Internal/ModioPrivateDefines.h"
#include "ModioSDK.h"
#include "Types/ModioAuthenticationParams.h"

// clang-format off
#include "ModioCommonTypes.generated.h"
// clang-format on

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
UENUM()
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
	constexpr explicit FModioModID(Modio::ModID InModId) : ModID(InModId) {}

	FORCEINLINE friend uint32 GetTypeHash(FModioModID ModioModId)
	{
		return FCrc::MemCrc32(&ModioModId.ModID, sizeof(Modio::ModID));
	}

	FORCEINLINE friend bool operator==(FModioModID A, FModioModID B)
	{
		return A.ModID == B.ModID;
	}

	FORCEINLINE friend bool operator!=(FModioModID A, FModioModID B)
	{
		return A.ModID != B.ModID;
	}

	FORCEINLINE friend bool operator<(FModioModID A, FModioModID B)
	{
		return A.ModID < B.ModID;
	}

	FORCEINLINE friend bool operator>(FModioModID A, FModioModID B)
	{
		return A.ModID > B.ModID;
	}

	/** Implicitly convert it to the underlying type */
	constexpr FORCEINLINE operator Modio::ModID() const
	{
		return ModID;
	}

	FORCEINLINE FString ToString() const
	{
		if (ModID < 0)
		{
			return TEXT("InvalidModID");
		}
		return FString::Printf(TEXT("%lld"), *ModID);
	}

private:
	Modio::ModID ModID;
};

template<>
struct TStructOpsTypeTraits<FModioModID> : public TStructOpsTypeTraitsBase2<FModioModID>
{
	enum
	{
		WithIdenticalViaEquality = true
	};
};

/** @brief Strong type for Game IDs */
USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeGameId"))
struct MODIO_API FModioGameID
{
	GENERATED_BODY()

	FModioGameID();
	constexpr explicit FModioGameID(Modio::GameID InGameId) : GameID(InGameId) {}
	constexpr explicit FModioGameID(int64 InGameId) : GameID(Modio::GameID(InGameId)) {}

	FORCEINLINE friend uint32 GetTypeHash(FModioGameID ModioGameId)
	{
		return FCrc::MemCrc32(&ModioGameId.GameID, sizeof(Modio::GameID));
	}

	FORCEINLINE friend bool operator==(FModioGameID A, FModioGameID B)
	{
		return A.GameID == B.GameID;
	}

	/** Implicitly convert it to the underlying type */
	constexpr FORCEINLINE operator Modio::GameID() const
	{
		return GameID;
	}

	FORCEINLINE FString ToString() const
	{
		if (GameID < 0)
		{
			return TEXT("InvalidGameID");
		}
		return FString::Printf(TEXT("%lld"), *GameID);
	}

	static FORCEINLINE FModioGameID InvalidGameID()
	{
		return FModioGameID(Modio::GameID::InvalidGameID());
	}

private:
	Modio::GameID GameID;
};

template<>
struct TStructOpsTypeTraits<FModioGameID> : public TStructOpsTypeTraitsBase2<FModioGameID>
{
	enum
	{
		WithIdenticalViaEquality = true
	};
};

/** @brief Strong type for File Metadata IDs */
USTRUCT(BlueprintType)
struct MODIO_API FModioFileMetadataID
{
	GENERATED_BODY()

	FModioFileMetadataID();
	constexpr explicit FModioFileMetadataID(Modio::FileMetadataID InFileMetadataId) : FileMetadataID(InFileMetadataId) {}

	FORCEINLINE friend uint32 GetTypeHash(FModioFileMetadataID FileMetadataID)
	{
		return FCrc::MemCrc32(&FileMetadataID.FileMetadataID, sizeof(Modio::FileMetadataID));
	}

	FORCEINLINE friend bool operator==(FModioFileMetadataID A, FModioFileMetadataID B)
	{
		return A.FileMetadataID == B.FileMetadataID;
	}
	
	/** Implicitly convert it to the underlying type */
	constexpr FORCEINLINE operator Modio::FileMetadataID() const
	{
		return FileMetadataID;
	}

	FORCEINLINE FString ToString() const
	{
		if (FileMetadataID < 0)
		{
			return TEXT("InvalidFileMetadataID");
		}
		return FString::Printf(TEXT("%lld"), *FileMetadataID);
	}

private:
	Modio::FileMetadataID FileMetadataID;
};

template<>
struct TStructOpsTypeTraits<FModioFileMetadataID> : public TStructOpsTypeTraitsBase2<FModioFileMetadataID>
{
	enum
	{
		WithIdenticalViaEquality = true
	};
};

/** @brief Strong type for User IDs */
USTRUCT(BlueprintType)
struct MODIO_API FModioUserID
{
	GENERATED_BODY()

	FModioUserID();
	constexpr explicit FModioUserID(Modio::UserID InUserID) : UserID(InUserID) {}
	constexpr explicit FModioUserID(int64 InUserID) : UserID(Modio::UserID(InUserID)) {}

	FORCEINLINE friend uint32 GetTypeHash(FModioUserID UserID)
	{
		return FCrc::MemCrc32(&UserID.UserID, sizeof(Modio::UserID));
	}

	FORCEINLINE friend bool operator==(FModioUserID A, FModioUserID B)
	{
		return A.UserID == B.UserID;
	}

	/** Implicitly convert it to the underlying type */
	constexpr FORCEINLINE operator Modio::UserID() const
	{
		return UserID;
	}

	FORCEINLINE FString ToString() const
	{
		if (UserID < 0)
		{
			return TEXT("InvalidUserID");
		}
		return FString::Printf(TEXT("%lld"), *UserID);
	}

private:
	Modio::UserID UserID;
};

template<>
struct TStructOpsTypeTraits<FModioUserID> : public TStructOpsTypeTraitsBase2<FModioUserID>
{
	enum
	{
		WithIdenticalViaEquality = true
	};
};

/** @brief Strong type for Api Keys */
USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeApiKey"))
struct MODIO_API FModioApiKey
{
	GENERATED_BODY()

	FModioApiKey() = default;
	explicit FModioApiKey(const FString& InApiKey);
	explicit FModioApiKey(const Modio::ApiKey& InApiKey);

	/** Implicitly convert it to the underlying type */
	FORCEINLINE operator Modio::ApiKey() const
	{
		return Modio::ApiKey(TCHAR_TO_UTF8(*ToString()));
	}

	FORCEINLINE const FString& ToString() const
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

	static FModioApiKey InvalidAPIKey()
	{
		return FModioApiKey(Modio::ApiKey::InvalidAPIKey());
	}

private:
	FString ApiKey;
};

/** @brief Strong type for email address */
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAddress
{
	GENERATED_BODY()

	FModioEmailAddress() = default;
	FModioEmailAddress(const FString& InEmailAddress);

	/** Implicitly convert it to the underlying type */
	FORCEINLINE operator Modio::EmailAddress() const
	{
		return Modio::EmailAddress(TCHAR_TO_UTF8(*ToString()));
	}

	FORCEINLINE const FString& ToString() const
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

	/** Implicitly convert it to the underlying type */
	FORCEINLINE operator Modio::EmailAuthCode() const
	{
		return Modio::EmailAuthCode(TCHAR_TO_UTF8(*ToString()));
	}

	FORCEINLINE const FString& ToString() const
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

FORCEINLINE FModioModID ToUnreal(Modio::ModID Value);
FORCEINLINE FModioFileMetadataID ToUnreal(Modio::FileMetadataID Value);

FORCEINLINE Modio::LogLevel ToModio(EModioLogLevel UnrealLogLevel);
FORCEINLINE Modio::AvatarSize ToModio(EModioAvatarSize AvatarSize);
FORCEINLINE Modio::GallerySize ToModio(EModioGallerySize GallerySize);
FORCEINLINE Modio::LogoSize ToModio(EModioLogoSize LogoSize);
FORCEINLINE Modio::Environment ToModio(EModioEnvironment Environment);
FORCEINLINE Modio::Portal ToModio(EModioPortal Portal);


#pragma region ToModio implementation

MODIO_BEGIN_CONVERT_SWITCHES

Modio::LogLevel ToModio(EModioLogLevel UnrealLogLevel)
{
	switch (UnrealLogLevel)
	{
		case EModioLogLevel::Error:
			return Modio::LogLevel::Error;
		case EModioLogLevel::Info:
			return Modio::LogLevel::Info;
		case EModioLogLevel::Trace:
			return Modio::LogLevel::Trace;
		case EModioLogLevel::Warning:
			return Modio::LogLevel::Warning;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLogLevel UnrealLogLevel)"));
	return Modio::LogLevel::Info;
}

Modio::AvatarSize ToModio(EModioAvatarSize AvatarSize)
{
	switch (AvatarSize)
	{
		case EModioAvatarSize::Original:
			return Modio::AvatarSize::Original;
		case EModioAvatarSize::Thumb50:
			return Modio::AvatarSize::Thumb50;
		case EModioAvatarSize::Thumb100:
			return Modio::AvatarSize::Thumb100;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioAvatarSize AvatarSize)"));
	return Modio::AvatarSize::Thumb50;
}

Modio::GallerySize ToModio(EModioGallerySize GallerySize)
{
	switch (GallerySize)
	{
		case EModioGallerySize::Original:
			return Modio::GallerySize::Original;
		case EModioGallerySize::Thumb320:
			return Modio::GallerySize::Thumb320;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioGallerySize GallerySize)"));
	return Modio::GallerySize::Thumb320;
}

Modio::LogoSize ToModio(EModioLogoSize LogoSize)
{
	switch (LogoSize)
	{
		case EModioLogoSize::Original:
			return Modio::LogoSize::Original;
		case EModioLogoSize::Thumb320:
			return Modio::LogoSize::Thumb320;
		case EModioLogoSize::Thumb640:
			return Modio::LogoSize::Thumb640;
		case EModioLogoSize::Thumb1280:
			return Modio::LogoSize::Thumb1280;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLogoSize LogoSize)"));
	return Modio::LogoSize::Thumb320;
}

Modio::Environment ToModio(EModioEnvironment Environment)
{
	switch (Environment)
	{
		case EModioEnvironment::Test:
			return Modio::Environment::Test;
		case EModioEnvironment::Live:
			return Modio::Environment::Live;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioEnvironment Environment)"));
	return Modio::Environment::Test;
}

Modio::Portal ToModio(EModioPortal Portal) {
	switch (Portal)
	{
		case EModioPortal::None:
			return Modio::Portal::None;
		break;
		case EModioPortal::Apple:
			return Modio::Portal::Apple;
		break;
		case EModioPortal::EpicGamesStore:
			return Modio::Portal::EpicGamesStore;
		break;
		case EModioPortal::GOG:
			return Modio::Portal::GOG;
		break; 
		case EModioPortal::Google:
			return Modio::Portal::Google;
		break;
		case EModioPortal::Itchio:
			return Modio::Portal::Itchio;
		break;
		case EModioPortal::Nintendo:
			return Modio::Portal::Nintendo;
		break;
		case EModioPortal::PSN:
			return Modio::Portal::PSN;
		break;
		case EModioPortal::Steam:
			return Modio::Portal::Steam;
		break;
		case EModioPortal::XboxLive:
			return Modio::Portal::XboxLive;
		break;
	}
	checkf(false, TEXT("Missed a case in ToModio(EModioPortal)"));
	return Modio::Portal::None;
}


FORCEINLINE Modio::Language ToModio(EModioLanguage Language)
{
	switch (Language)
	{
		case EModioLanguage::English:
			return Modio::Language::English;
		case EModioLanguage::Bulgarian:
			return Modio::Language::Bulgarian;
		case EModioLanguage::French:
			return Modio::Language::French;
		case EModioLanguage::German:
			return Modio::Language::German;
		case EModioLanguage::Italian:
			return Modio::Language::Italian;
		case EModioLanguage::Polish:
			return Modio::Language::Polish;
		case EModioLanguage::Portuguese:
			return Modio::Language::Portuguese;
		case EModioLanguage::Hungarian:
			return Modio::Language::Hungarian;
		case EModioLanguage::Japanese:
			return Modio::Language::Japanese;
		case EModioLanguage::Korean:
			return Modio::Language::Korean;
		case EModioLanguage::Russian:
			return Modio::Language::Russian;
		case EModioLanguage::Spanish:
			return Modio::Language::Spanish;
		case EModioLanguage::Thai:
			return Modio::Language::Thai;
		case EModioLanguage::ChineseSimplified:
			return Modio::Language::ChineseSimplified;
		case EModioLanguage::ChineseTraditional:
			return Modio::Language::ChineseTraditional;
	}

	checkf(false, TEXT("Missed a case in ToModio(EModioLanguage Language)"));
	return Modio::Language::English;
}


MODIO_END_CONVERT_SWITCHES

FModioModID ToUnreal(Modio::ModID Value)
{
	return FModioModID(Value);
}

FModioFileMetadataID ToUnreal(Modio::FileMetadataID Value)
{
	return FModioFileMetadataID(Value);
}

#pragma endregion
