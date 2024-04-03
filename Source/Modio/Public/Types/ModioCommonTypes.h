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
	struct EntitlementParams;
} // namespace Modio

/**
 * Enum representing what environment the game is deployed in, between
 * test or a live environment
 **/
UENUM(BlueprintType)
enum class EModioEnvironment : uint8
{
	/** Test/Private environment **/
	Test,

	/** Live/Public environment **/
	Live
};

/**
 * Enum representing the store or service your game is being distributed through
 **/
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
	XboxLive,
	Android
};

/**
 * Enum representing a named platform that the plugin is running on.
 **/
UENUM(BlueprintType)
enum class EModioPlatformName : uint8
{
	Windows,
	Mac,
	Linux,
	PS4,
	PS5,
	XBoxOne,
	XSX,
	Switch,
	Unknown,
	Android,
	iOS
};


/**
 * Enum representing the platform(s) that a modfile is enabled for
 **/
UENUM(BlueprintType)
enum class EModioModfilePlatform : uint8
{
	Windows,
	Mac,
	Linux,
	Android,
	iOS,
	XboxOne,
	XboxSeriesX,
	PS4,
	PS5,
	Switch,
	Oculus,
	Source
};

/**
 * Enum representing mod logo sizes
 **/
UENUM(BlueprintType)
enum class EModioLogoSize : uint8
{
	/** 320x180px **/
	Thumb320 = 0,

	/** 640x360px **/
	Thumb640 = 1,

	/** 1280x720px **/
	Thumb1280 = 2,

	/** Original Size **/
	Original = 3
};

/**
 * Enum representing avatar image sizes
 **/
UENUM(BlueprintType)
enum class EModioAvatarSize : uint8
{
	/** Original Size **/
	Original,

	/** 50x50px Thumbnail **/
	Thumb50,

	/** 100x100px Thumbnail **/
	Thumb100
};

/**
 * Enumerator for the standard size a gallery image can use
 **/
UENUM(BlueprintType)
enum class EModioGallerySize : uint8
{
	/** Original Size **/
	Original,

	/** 320x180px Thumbnail **/
	Thumb320,

	/** 1280x720 Thumbnail **/
	Thumb1280
};

/**
 * Degree of severity for the log output
 **/
UENUM(BlueprintType)
enum class EModioLogLevel : uint8
{
	/** Detailed low-level debugging output. Not intended for general use **/
	Trace = 0,

	/** Informational output containing status messages **/
	Info = 1,

	/** Warnings about incorrect plugin usage, timeouts **/
	Warning = 2,

	/** Only errors **/
	Error = 3
};

/**
 * Enum representing the languages mod.io support responses in
 **/
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

UENUM(BlueprintType)
enum class EModioModChangeType : uint8
{
	Added, // The user's list has a new mod to synchronize
	Removed, // The user's list must remove a mod to synchronize
	Updated // The user's list must update a mod to synchronize
};

/**
 * Strong type struct to wrap a ModID to uniquely identify user generated content
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioModID
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioModID();

	/**
	 * Preferred constructor with ModID initialization parameter
	 * @param InModId Base ModID to create this strong type
	 **/
	constexpr explicit FModioModID(int64 InModId) : ModID(InModId) {}

	/**
	 * Transform a ModID into its 32 bit integer representation
	 * @param UserID String type for the ModID to read its hash from
	 * @return unsigned 32 bit integer that matches the hash of the ModID provided
	 **/
	MODIO_API friend uint32 GetTypeHash(FModioModID ModioModId);

	/**
	 * Comparison operator between ModID elements
	 **/
	MODIO_API friend bool operator==(FModioModID A, FModioModID B)
	{
		return A.ModID == B.ModID;
	}

	/**
	 * Comparison operator between ModID elements
	 **/
	MODIO_API friend bool operator!=(FModioModID A, FModioModID B)
	{
		return A.ModID != B.ModID;
	}

	/**
	 * Less than operator between ModID elements
	 **/
	friend bool operator<(FModioModID A, FModioModID B)
	{
		return A.ModID < B.ModID;
	}

	/**
	 * More than operator between ModID elements
	 **/
	friend bool operator>(FModioModID A, FModioModID B)
	{
		return A.ModID > B.ModID;
	}

	/**
	 * Transform a ModID into its string representation
	 * @return String value of the stored ModID
	 **/
	FString ToString() const
	{
		if (ModID < 0)
		{
			return TEXT("InvalidModID");
		}
		return FString::Printf(TEXT("%lld"), ModID);
	}

	/**
	 * Stream forward operator to pass the ModID along
	 * @param Ar The archive class that receives information
	 * @param ID The ModID to pass along this operator
	 * @return FArchive The updated archive with the ModID passed along
	 **/
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioModID& ID)
	{
		return Ar << ID.ModID;
	}

	/**
	 * Store this instance ModID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the ModID is forwarded to Ar
	 **/
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * Store this instance ModID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the ModID is forwarded to Ar
	 **/
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	/// @brief Function for retrieving the underlying value of an FModioModID for use in custom serialization. Not
	/// recommended for any other use. FModioModIDs should be treated as opaque.
	/// @param In the FModioModID to retrieve the value for
	/// @return the underlying numeric ID.
	MODIO_API friend int64 GetUnderlyingValue(const FModioModID& In)
	{
		return In.ModID;
	}

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

/**
 * Strong type struct to wrap ModID as an optional value
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModID
{
	GENERATED_BODY()

	/**
	 * Stored optional ModID
	 **/
	TOptional<FModioModID> Internal;
};

/**
 * Strong type struct to wrap a GameID to uniquely identify a single game in mod.io
 **/
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeGameId"))
struct MODIO_API FModioGameID
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioGameID();

	/**
	 * Preferred constructor with GameID initialization parameter
	 * @param InGameId Base GameID to create this strong type
	 **/
	constexpr explicit FModioGameID(int64 InGameId) : GameId(InGameId) {}

	/**
	 * Transform a GameID into its 32 bit integer representation
	 * @param ModGameId String type for the GameID to read its hash from
	 * @return unsigned 32 bit integer that matches the hash of the GameID provided
	 **/
	MODIO_API friend uint32 GetTypeHash(FModioGameID ModioGameId);

	/**
	 * Comparison operator between GameID elements
	 **/
	MODIO_API friend bool operator==(FModioGameID A, FModioGameID B)
	{
		return A.GameId == B.GameId;
	}

	/**
	 * Transform a GameID into its string representation
	 * @return String value of the stored GameID
	 **/
	FString ToString() const
	{
		if (GameId < 0)
		{
			return TEXT("InvalidGameID");
		}
		return FString::Printf(TEXT("%lld"), GameId);
	}

	/**
	 * An always invalid GameID, helpful to compare against other GameIDs
	 * @return String value of the stored GameID
	 **/
	static FModioGameID InvalidGameID();

	/**
	 * Stream forward operator to pass the GameID along
	 * @param Ar The archive class that receives information
	 * @param ID The GameID to pass along this operator
	 * @return FArchive The updated archive with the GameID passed along
	 **/
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioGameID& ID)
	{
		return Ar << ID.GameId;
	}

	/**
	 * Store this instance GameID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the GameID is forwarded to Ar
	 **/
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * Store this instance GameID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the GameID is forwarded to Ar
	 **/
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

/**
 * Strong type struct to store the associated metadata to a mod file
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioFileMetadataID
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioFileMetadataID();

	/**
	 * Preferred constructor with FileMetadadataID initialization parameter
	 * @param InFileMetadataId Base FileMetadataID to create this strong type
	 **/
	constexpr explicit FModioFileMetadataID(int64 InFileMetadataId) : FileMetadataID(InFileMetadataId) {}

	/**
	 * Transform a FileMetadataID into its 32 bit integer representation
	 * @param FileMetadataID String type for the FileMetadataID to read its hash from
	 * @return unsigned 32 bit integer that matches the hash of the FileMetadataID provided
	 **/
	MODIO_API friend uint32 GetTypeHash(FModioFileMetadataID FileMetadataID);

	/**
	 * Comparison operator between FileMetadataID elements
	 **/
	MODIO_API friend bool operator==(FModioFileMetadataID A, FModioFileMetadataID B)
	{
		return A.FileMetadataID == B.FileMetadataID;
	}

	/**
	 * Transform a FileMetadataID into its string representation
	 * @return String value of the stored FileMetadataID
	 **/
	FString ToString() const
	{
		if (FileMetadataID < 0)
		{
			return TEXT("InvalidFileMetadataID");
		}
		return FString::Printf(TEXT("%lld"), FileMetadataID);
	}

	/**
	 * Stream forward operator to pass the FileMetadataID along
	 * @param Ar The archive class that receives information
	 * @param ID The FileMetadataID to pass along this operator
	 * @return FArchive The updated archive with the FileMetadataID passed along
	 **/
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioFileMetadataID& ID)
	{
		return Ar << ID.FileMetadataID;
	}

	/**
	 * Store this instance FileMetadataID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the FileMetadataID is forwarded to Ar
	 **/
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * Store this instance FileMetadataID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the FileMetadataID is forwarded to Ar
	 **/
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

/**
 * Strong type struct to wrap a UserID which uniquely identifies a user in mod.io
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioUserID
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioUserID();

	/**
	 * Preferred constructor with UserID initialization parameter
	 * @param InUserID Base InUserID to create this strong type
	 **/
	constexpr explicit FModioUserID(int64 InUserID) : UserID(InUserID) {}

	/**
	 * Transform a UserID into its 32 bit integer representation
	 * @param UserID String type for the UserID to read its hash from
	 * @return unsigned 32 bit integer that matches the hash of the UserID provided
	 **/
	MODIO_API friend uint32 GetTypeHash(FModioUserID UserID);

	/**
	 * Comparison operator between UserID elements
	 **/
	MODIO_API friend bool operator==(FModioUserID A, FModioUserID B)
	{
		return A.UserID == B.UserID;
	}

	/**
	 * Transform a UserID into its string representation
	 * @return String value of the stored UserID
	 **/
	FString ToString() const
	{
		if (UserID < 0)
		{
			return TEXT("InvalidUserID");
		}
		return FString::Printf(TEXT("%lld"), UserID);
	}

	/**
	 * Stream forward operator to pass the UserID along
	 * @param Ar The archive class that receives information
	 * @param ID The UserID to pass along this operator
	 * @return FArchive The updated archive with the UserID passed along
	 **/
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioUserID& ID)
	{
		return Ar << ID.UserID;
	}

	/**
	 * Store this instance UserID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the UserID is forwarded to Ar
	 **/
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * Store this instance UserID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the UserID is forwarded to Ar
	 **/
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

/**
 * Strong type struct to wrap an ApiKey, used to communicate with the mod.io service
 **/
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeApiKey"))
struct MODIO_API FModioApiKey
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioApiKey() = default;

	/**
	 * Preferred constructor with ApiKey initialization parameter
	 * @param InApiKey Base ApiKey to create this strong type
	 **/
	explicit FModioApiKey(const FString& InApiKey);

	/**
	 * Transform a ApiKey into its string representation
	 * @return String value of the stored ApiKey
	 **/
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

	/**
	 * An always invalid APIKey, helpful to compare against other APIKeys
	 * @return String value of the stored APIKey
	 **/
	static FModioApiKey InvalidAPIKey();

private:
	friend struct FModioApiKeyStructCustomization;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	FString ApiKey;
};

/**
 * Strong type struct to wrap an email address
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAddress
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioEmailAddress() = default;

	/**
	 * Preferred constructor with EmailAddress initialization parameter
	 * @param InEmailAddress Base EmailAddress to create this strong type
	 **/
	FModioEmailAddress(const FString& InEmailAddress);

	/**
	 * Transform a EmailAddress into its string representation
	 * @return String value of the stored EmailAddress
	 **/
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

/**
 * Strong type struct to wrap the code sent to the email address to obtain an authentication token
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAuthCode
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioEmailAuthCode() = default;

	/**
	 * Preferred constructor with EmailAuthCode initialization parameter
	 * @param InEmailAuthCode Base EmailAuthCode to create this strong type
	 **/
	FModioEmailAuthCode(const FString& InEmailAuthCode);

	/**
	 * Transform a EmailAuthCode into its string representation
	 * @return String value of the stored EmailAuthCode
	 **/
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

/**
 * Simple struct to encapsulate portal-specific data required when consuming entitlements
 **/
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeEntitlementParams"))
struct MODIO_API FModioEntitlementParams
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioEntitlementParams() = default;

	/**
	 * Preferred constructor with ExtendedParameters initialization parameter
	 * @param InExtendedParameters Base ExtendedParameters to create this struct
	 **/
	explicit FModioEntitlementParams(const TMap<FString, FString>& InExtendedParameters) : ExtendedParameters(InExtendedParameters) {}

private:
	friend struct Modio::EntitlementParams ToModio(const FModioEntitlementParams& In);
	/// @brief ExtendedParameters A map to store extended parameters required by some portals.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	TMap<FString, FString> ExtendedParameters;
};

#pragma endregion
