/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Misc/Crc.h"
#include "Misc/EnumRange.h"

#include "ModioCommonTypes.generated.h"

// Forward decls
namespace Modio
{
	struct ModID;
	struct ModCollectionID;
	struct GameID;
	struct FileMetadataID;
	struct UserID;
	struct EntitlementParams;
	struct MetricsSessionParams;
	struct Guid;
} // namespace Modio

/**
 * @brief Enum representing which environment the game is deployed to: `Test` or `Live`.
 */
UENUM(BlueprintType)
enum class EModioEnvironment : uint8
{
	/** Test (private) environment */
	Test,

	/** Live (public) environment */
	Live
};

/**
 * @brief Enum representing the store or service your game is being distributed through
 */
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
};

/**
 * @brief Enum representing a named platform that the plugin is running on.
 */
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
 * @brief Enum representing the platform(s) that a modfile is enabled for
 */
UENUM(BlueprintType)
enum class EModioModfilePlatform : uint8
{
	Windows UMETA(DisplayName = "Windows"),
	Mac UMETA(DisplayName = "Mac"),
	Linux UMETA(DisplayName = "Linux"),
	Android UMETA(DisplayName = "Android"),
	iOS UMETA(DisplayName = "iOS"),
	XboxOne UMETA(DisplayName = "Xbox One"),
	XboxSeriesX UMETA(DisplayName = "Xbox Series X"),
	PS4 UMETA(DisplayName = "PS4"),
	PS5 UMETA(DisplayName = "PS5"),
	Switch UMETA(DisplayName = "Switch"),
	Oculus UMETA(DisplayName = "Oculus"),
	Source UMETA(DisplayName = "Source"),
	WindowsServer UMETA(DisplayName = "Windows Server"),
	LinuxServer UMETA(DisplayName = "Linux Server"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EModioModfilePlatform, EModioModfilePlatform::Count);

/**
 * Enum representing mod logo sizes
 */
UENUM(BlueprintType)
enum class EModioLogoSize : uint8
{
	/** 320x180px */
	Thumb320 = 0,
	/** 640x360px */
	Thumb640 = 1,
	/** 1280x720px */
	Thumb1280 = 2,
	/** Original Size */
	Original = 3
};

/**
 * Enum representing avatar image sizes
 */
UENUM(BlueprintType)
enum class EModioAvatarSize : uint8
{
	/** Original Size */
	Original,
	/** 50x50px Thumbnail */
	Thumb50,
	/** 100x100px Thumbnail */
	Thumb100
};

/**
 * Enumerator for the standard size a gallery image can use
 */
UENUM(BlueprintType)
enum class EModioGallerySize : uint8
{
	/** Original Size */
	Original,

	/** 320x180px Thumbnail */
	Thumb320,

	/** 1280x720 Thumbnail */
	Thumb1280
};

/**
 * Degree of severity for the log output
 */
UENUM(BlueprintType)
enum class EModioLogLevel : uint8
{
	/** Detailed low-level debugging output. Not intended for general use */
	Trace = 0,

	/* Detailed but not low-level. Generally useful for some mid-level information for debugging. */
	Detailed = 1,

	/** Informational output containing status messages */
	Info = 2,

	/** Warnings about incorrect plugin usage, timeouts */
	Warning = 3,

	/** Only errors */
	Error = 4
};

/**
 * @docpublic
 * @brief Enum representing the languages mod.io supports responses in
 */
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
	ChineseTraditional,
	Turkish,
	Ukrainian,
	Arabic,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EModioLanguage, EModioLanguage::Count);

/**
 * @docpublic
 * @brief Enum representing the types of changes that can occur to a mod in the user's list
 */
UENUM(BlueprintType)
enum class EModioModChangeType : uint8
{
	// The user's list has a new mod to synchronize
	Added,
	// The user's list must remove a mod to synchronize
	Removed,
	// The user's list must update a mod to synchronize
	Updated
};

/**
 * @docpublic
 * @brief Strong type struct to wrap a ModID to uniquely identify user-generated content
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModID
{
	GENERATED_BODY()

	/**
	 * @brief Default constructor without parameters
	 */
	FModioModID();

	/**
	 * @docpublic
	 * @brief Preferred constructor with ModID initialization parameter
	 * @param InModId Base ModID to create this strong type
	 */
	constexpr explicit FModioModID(int64 InModId) : ModID(InModId) {}

	/**
	 * @docpublic
	 * @brief Transform a ModID into its 32-bit integer representation
	 * @param ModioModId String type for the ModID to read its hash from
	 * @return unsigned 32-bit integer that matches the hash of the ModID provided
	 */
	MODIO_API friend uint32 GetTypeHash(FModioModID ModioModId);

	/**
	 * @docpublic
	 * @brief Comparison operator between ModID elements
	 */
	MODIO_API friend bool operator==(FModioModID A, FModioModID B)
	{
		return A.ModID == B.ModID;
	}

	/**
	 * @docpublic
	 * @brief Comparison operator between ModID elements
	 */
	MODIO_API friend bool operator!=(FModioModID A, FModioModID B)
	{
		return A.ModID != B.ModID;
	}

	/**
	 * @docpublic
	 * @brief Less than operator between ModID elements
	 */
	friend bool operator<(FModioModID A, FModioModID B)
	{
		return A.ModID < B.ModID;
	}

	/**
	 * @docpublic
	 * @brief Greater than operator between ModID elements
	 */
	friend bool operator>(FModioModID A, FModioModID B)
	{
		return A.ModID > B.ModID;
	}

	/**
	 * @docpublic
	 * @brief Transform a ModID into its string representation
	 * @return String value of the stored ModID
	 */
	FString ToString() const
	{
		return FString::Printf(TEXT("%lld"), ModID);
	}

	/**
	 * @docinternal
	 * @brief Compare the ModID to the invalid state
	 * @return True if ModID is not invalid (INDEX_NONE)
	 */
	bool IsValid() const
	{
		return ModID != INDEX_NONE;
	}

	/**
	 * @docpublic
	 * @brief Stream forward operator to pass the ModID along
	 * @param Ar The archive class that receives information
	 * @param ID The ModID to pass along this operator
	 * @return FArchive The updated archive with the ModID passed along
	 */
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioModID& ID)
	{
		return Ar << ID.ModID;
	}

	/**
	 * @docpublic
	 * @brief Store this instance ModID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the ModID is forwarded to Ar
	 */
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * @docpublic
	 * @brief Store this instance ModID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the ModID is forwarded to Ar
	 */
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	/**
	 * @brief Function for retrieving the underlying value of an FModioModID for use in custom serialization. Not
	 * recommended for any other use. FModioModIDs should be treated as opaque.
	 * @param In The FModioModID to retrieve the value for
	 * @return The underlying numeric ID
	 */
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
 * @docpublic
 * @brief Strong type struct to wrap ModID as an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModID
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored optional ModID
	 */
	TOptional<FModioModID> Internal;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap uint64 as an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUInt64
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored optional uint64
	 */
	TOptional<uint64> Internal;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap a ModCollectionID to uniquely identify collections of user-generated content.
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionID
{
	GENERATED_BODY()

	/**
	 * @brief Default constructor without parameters
	 */
	FModioModCollectionID();

	/**
	 * @docpublic
	 * @brief Preferred constructor with ModCollectionID initialization parameter
	 * @param ModioModCollectionId Base ModCollectionID to create this strong type
	 */
	constexpr explicit FModioModCollectionID(int64 ModioModCollectionId) : ModCollectionID(ModioModCollectionId) {}

	/**
	 * @docpublic
	 * @brief Transform a ModCollectionID into its 32-bit integer representation
	 * @param ModioModCollectionId String type for the ModCollectionID to read its hash from
	 * @return unsigned 32-bit integer that matches the hash of the ModCollectionID provided
	 */
	MODIO_API friend uint32 GetTypeHash(FModioModCollectionID ModioModCollectionId);

	/**
	 * @docpublic
	 * @brief Comparison operator between ModCollectionID elements
	 */
	MODIO_API friend bool operator==(FModioModCollectionID A, FModioModCollectionID B)
	{
		return A.ModCollectionID == B.ModCollectionID;
	}

	/**
	 * @docpublic
	 * @brief Comparison operator between ModCollectionID elements
	 */
	MODIO_API friend bool operator!=(FModioModCollectionID A, FModioModCollectionID B)
	{
		return A.ModCollectionID != B.ModCollectionID;
	}

	/**
	 * @docpublic
	 * @brief Less than operator between ModCollectionID elements
	 */
	friend bool operator<(FModioModCollectionID A, FModioModCollectionID B)
	{
		return A.ModCollectionID < B.ModCollectionID;
	}

	/**
	 * @docpublic
	 * @brief Greater than operator between ModCollectionID elements
	 */
	friend bool operator>(FModioModCollectionID A, FModioModCollectionID B)
	{
		return A.ModCollectionID > B.ModCollectionID;
	}

	/**
	 * @docpublic
	 * @brief Transform a ModCollectionID into its string representation
	 * @return String value of the stored ModCollectionID
	 */
	FString ToString() const
	{
		return FString::Printf(TEXT("%lld"), ModCollectionID);
	}

	/**
	 * @docinternal
	 * @brief Compare the ModCollectionID to the invalid state
	 * @return True if ModCollectionID is not invalid (INDEX_NONE)
	 */
	bool IsValid() const
	{
		return ModCollectionID != INDEX_NONE;
	}

	/**
	 * @docpublic
	 * @brief Stream forward operator to pass the ModCollectionID along
	 * @param Ar The archive class that receives information
	 * @param ID The ModCollectionID to pass along this operator
	 * @return FArchive The updated archive with the ModCollectionID passed along
	 */
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioModCollectionID& ID)
	{
		return Ar << ID.ModCollectionID;
	}

	/**
	 * @docpublic
	 * @brief Store this instance ModCollectionID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the ModCollectionID is forwarded to Ar
	 */
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * @docpublic
	 * @brief Store this instance ModCollectionID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the ModCollectionID is forwarded to Ar
	 */
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	/**
	 * @brief Function for retrieving the underlying value of an FModioModCollectionID for use in custom serialization. Not
	 * recommended for any other use. FModioModCollectionIDs should be treated as opaque.
	 * @param In The FModioModCollectionID to retrieve the value for
	 * @return The underlying numeric ID
	 */
	MODIO_API friend int64 GetUnderlyingValue(const FModioModCollectionID& In)
	{
		return In.ModCollectionID;
	}

	friend struct Modio::ModCollectionID ToModio(const FModioModCollectionID& In);
	int64 ModCollectionID;
};

template<>
struct TStructOpsTypeTraits<FModioModCollectionID> : public TStructOpsTypeTraitsBase2<FModioModCollectionID>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithNetSerializer = true
	};
};

/**
 * @docpublic
 * @brief Strong type struct to wrap a GameID to uniquely identify a single game in mod.io
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeGameId"))
struct MODIO_API FModioGameID
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioGameID();

	/**
	 * @docpublic
	 * @brief Preferred constructor with GameID initialization parameter
	 * @param InGameId Base GameID to create this strong type
	 */
	constexpr explicit FModioGameID(int64 InGameId) : GameId(InGameId) {}

	/**
	 * @docpublic
	 * @brief Transform a GameID into its 32-bit integer representation
	 * @param ModioGameId String type for the GameID to read its hash from
	 * @return unsigned 32-bit integer that matches the hash of the GameID provided
	 */
	MODIO_API friend uint32 GetTypeHash(FModioGameID ModioGameId);

	/**
	 * @docpublic
	 * @brief Comparison operator between GameID elements
	 */
	MODIO_API friend bool operator==(FModioGameID A, FModioGameID B)
	{
		return A.GameId == B.GameId;
	}

	/**
	 * @docpublic
	 * @brief Transform a GameID into its string representation
	 * @return String value of the stored GameID
	 */
	FString ToString() const
	{
		return FString::Printf(TEXT("%lld"), GameId);
	}

	/**
	 * @docinternal
	 * @brief Compare the GameID to the invalid state defined by the SDK
	 * @return True if the GameID is not in its invalid state
	 */
	bool IsValid() const
	{
		return GameId != InvalidGameID().GameId;
	}

	/**
	 * @docpublic
	 * @brief An always invalid GameID, helpful to compare against other GameIDs
	 * @return An invalid FModioGameID instance
	 */
	static FModioGameID InvalidGameID();

	/**
	 * @docpublic
	 * @brief Stream forward operator to pass the GameID along
	 * @param Ar The archive class that receives information
	 * @param ID The GameID to pass along this operator
	 * @return FArchive The updated archive with the GameID passed along
	 */
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioGameID& ID)
	{
		return Ar << ID.GameId;
	}

	/**
	 * @docpublic
	 * @brief Store this instance GameID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the GameID is forwarded to Ar
	 */
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * @docpublic
	 * @brief Store this instance GameID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the GameID is forwarded to Ar
	 */
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
 * @docpublic
 * @brief Strong type struct to store the associated metadata to a mod file
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioFileMetadataID
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioFileMetadataID();

	/**
	 * @docpublic
	 * @brief Preferred constructor with FileMetadataID initialization parameter
	 * @param InFileMetadataId Base FileMetadataID to create this strong type
	 */
	constexpr explicit FModioFileMetadataID(int64 InFileMetadataId) : FileMetadataID(InFileMetadataId) {}

	/**
	 * @docpublic
	 * @brief Transform a FileMetadataID into its 32-bit integer representation
	 * @param FileMetadataID String type for the FileMetadataID to read its hash from
	 * @return unsigned 32-bit integer that matches the hash of the FileMetadataID provided
	 */
	MODIO_API friend uint32 GetTypeHash(FModioFileMetadataID FileMetadataID);

	/**
	 * @docpublic
	 * @brief Comparison operator between FileMetadataID elements
	 */
	MODIO_API friend bool operator==(FModioFileMetadataID A, FModioFileMetadataID B)
	{
		return A.FileMetadataID == B.FileMetadataID;
	}

	/**
	 * @docpublic
	 * @brief Transform a FileMetadataID into its string representation
	 * @return String value of the stored FileMetadataID
	 */
	FString ToString() const
	{
		return FString::Printf(TEXT("%lld"), FileMetadataID);
	}

	/**
	 * @docinternal
	 * @brief Compare the FileMetadataID to the invalid state
	 * @return True if the FileMetadataID is not in its invalid state
	 */
	bool IsValid() const
	{
		return FileMetadataID != INDEX_NONE;
	}

	/**
	 * @docpublic
	 * @brief Stream forward operator to pass the FileMetadataID along
	 * @param Ar The archive class that receives information
	 * @param ID The FileMetadataID to pass along this operator
	 * @return FArchive The updated archive with the FileMetadataID passed along
	 */
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioFileMetadataID& ID)
	{
		return Ar << ID.FileMetadataID;
	}

	/**
	 * @docpublic
	 * @brief Store this instance FileMetadataID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the FileMetadataID is forwarded to Ar
	 */
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * @docpublic
	 * @brief Store this instance FileMetadataID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the FileMetadataID is forwarded to Ar
	 */
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
 * @docpublic
 * @brief Strong type struct to wrap a UserID which uniquely identifies a user in mod.io
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioUserID
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioUserID();

	/**
	 * @docpublic
	 * @brief Preferred constructor with UserID initialization parameter
	 * @param InUserID Base InUserID to create this strong type
	 */
	constexpr explicit FModioUserID(int64 InUserID) : UserID(InUserID) {}

	/**
	 * @docpublic
	 * @brief Transform a UserID into its 32-bit integer representation
	 * @param UserID String type for the UserID to read its hash from
	 * @return unsigned 32-bit integer that matches the hash of the UserID provided
	 */
	MODIO_API friend uint32 GetTypeHash(FModioUserID UserID);

	/**
	 * @docpublic
	 * @brief Comparison operator between UserID elements
	 */
	MODIO_API friend bool operator==(FModioUserID A, FModioUserID B)
	{
		return A.UserID == B.UserID;
	}

	/**
	 * @docpublic
	 * @brief Transform a UserID into its string representation
	 * @return String value of the stored UserID
	 */
	FString ToString() const
	{
		return FString::Printf(TEXT("%lld"), UserID);
	}

	/**
	 * @docinternal
	 * @brief Compare the UserID to the invalid state
	 * @return True if the UserID is not in its invalid state
	 */
	bool IsValid() const
	{
		return UserID != INDEX_NONE;
	}

	/**
	 * @docpublic
	 * @brief Stream forward operator to pass the UserID along
	 * @param Ar The archive class that receives information
	 * @param ID The UserID to pass along this operator
	 * @return FArchive The updated archive with the UserID passed along
	 */
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioUserID& ID)
	{
		return Ar << ID.UserID;
	}

	/**
	 * @docpublic
	 * @brief Store this instance UserID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the UserID is forwarded to Ar
	 */
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * @docpublic
	 * @brief Store this instance UserID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the UserID is forwarded to Ar
	 */
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
 * @docpublic
 * @brief Strong type struct to wrap an ApiKey, used to communicate with the mod.io service
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeApiKey"))
struct MODIO_API FModioApiKey
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioApiKey() = default;

	/**
	 * @docpublic
	 * @brief Preferred constructor with ApiKey initialization parameter
	 * @param InApiKey Base ApiKey to create this strong type
	 */
	explicit FModioApiKey(const FString& InApiKey);

	/**
	 * @docpublic
	 * @brief Transform a ApiKey into its string representation
	 * @return String value of the stored ApiKey
	 */
	const FString& ToString() const
	{
		return ApiKey;
	}

	/**
	 * @docinternal
	 * @brief Compare the ApiKey to the invalid state defined by the SDK
	 * @return True if the ApiKey is not in its invalid state
	 */
	bool IsValid() const
	{
		return ApiKey != InvalidAPIKey().ToString();
	}

	/**
	 * @docpublic
	 * @brief An always invalid APIKey, helpful to compare against other APIKeys
	 * @return A default invalid ApiKey
	 */
	static FModioApiKey InvalidAPIKey();

private:
	friend struct FModioApiKeyStructCustomization;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	FString ApiKey;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap a Guid, used to communicate with the mod.io metrics service
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeGuid"))
struct MODIO_API FModioGuid
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioGuid() = default;

	/**
	 * @docpublic
	 * @brief Generates a new Guid.
	 * @note This is a minimalistic implementation of generating a Guid for metrics, an improved more robust
	 * per-platform implementation is planned
	 * @return A newly generated Guid
	 */
	static FModioGuid GenerateGuid();

	/**
	 * @docpublic
	 * @brief Preferred constructor with Guid initialization parameter
	 * @param InGuid Base Guid to create this strong type
	 */
	explicit FModioGuid(const FString& InGuid);

	/**
	 * @docpublic
	 * @brief Transform a Guid into its string representation
	 * @return String value of the stored Guid
	 */
	const FString& ToString() const
	{
		return InternalGuid;
	}

	/**
	 * @docinternal
	 * @brief Compare the InternalGuid to the invalid state defined by the SDK
	 * @return True if the Guid is not in its invalid state
	 */
	bool IsValid() const
	{
		return InternalGuid != InvalidGuid().InternalGuid;
	}

	/**
	 * @docpublic
	 * @brief An always invalid Guid, helpful to compare against other Guids
	 * @return Guid struct containing an invalid String value
	 */
	static FModioGuid InvalidGuid();

private:
	friend struct Modio::Guid ToModio(const FModioGuid& In);
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	FString InternalGuid;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap Guid as an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalGuid
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored optional Guid
	 */
	TOptional<FModioGuid> Internal;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap an email address
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAddress
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioEmailAddress() = default;

	/**
	 * @docpublic
	 * @brief Preferred constructor with EmailAddress initialization parameter
	 * @param InEmailAddress Base EmailAddress to create this strong type
	 */
	FModioEmailAddress(const FString& InEmailAddress);

	/**
	 * @docpublic
	 * @brief Transform a EmailAddress into its string representation
	 * @return String value of the stored EmailAddress
	 */
	const FString& ToString() const
	{
		return EmailAddress;
	}

	/**
	 * @docinternal
	 * @brief Compare the EmailAddress to the invalid state
	 * @return True if the EmailAddress has a non-empty value
	 */
	bool IsValid() const
	{
		return EmailAddress.Len() > 0;
	}

private:
	FString EmailAddress;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap the code sent to the email address to obtain an authentication token
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEmailAuthCode
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioEmailAuthCode() = default;

	/**
	 * @docpublic
	 * @brief Preferred constructor with EmailAuthCode initialization parameter
	 * @param InEmailAuthCode Base EmailAuthCode to create this strong type
	 */
	FModioEmailAuthCode(const FString& InEmailAuthCode);

	/**
	 * @docpublic
	 * @brief Transform a EmailAuthCode into its string representation
	 * @return String value of the stored EmailAuthCode
	 */
	const FString& ToString() const
	{
		return EmailAuthCode;
	}

	/**
	 * @docinternal
	 * @brief Compare the EmailAuthCode to the invalid state
	 * @return True if the EmailAuthCode has a non-empty value
	 */
	bool IsValid() const
	{
		return EmailAuthCode.Len() > 0;
	}

private:
	FString EmailAuthCode;
};

/**
 * @docpublic
 * @brief Simple struct to encapsulate portal-specific data required when consuming entitlements
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeEntitlementParams"))
struct MODIO_API FModioEntitlementParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioEntitlementParams() = default;

	/**
	 * @docpublic
	 * @brief Preferred constructor with ExtendedParameters initialization parameter
	 * @param InExtendedParameters Base ExtendedParameters to create this struct
	 */
	explicit FModioEntitlementParams(const TMap<FString, FString>& InExtendedParameters)
		: ExtendedParameters(InExtendedParameters)
	{}

private:
	friend struct Modio::EntitlementParams ToModio(const FModioEntitlementParams& In);

	/** @brief ExtendedParameters A map to store extended parameters required by some portals. */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	TMap<FString, FString> ExtendedParameters;
};

/**
 * @docpublic
 * @brief Simple struct to store metric session specific parameters
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeMetricsSessionParams"))
struct MODIO_API FModioMetricsSessionParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioMetricsSessionParams() = default;

	/**
	 * @docpublic
	 * @brief Preferred constructor with array of Mod Ids to track
	 * @param InIds Mod Id array to create this struct
	 */
	explicit FModioMetricsSessionParams(const TArray<FModioModID>& InIds) : SessionId({}), ModIds(InIds) {}

	/**
	 * @docpublic
	 * @brief Constructor with custom Session Id and array of Mod Ids to track
	 * @param InSessionId Custom Session Id to use
	 * @param InIds Mod Id array to create this struct
	 */
	explicit FModioMetricsSessionParams(const FModioGuid& InSessionId, const TArray<FModioModID>& InIds)
		: SessionId(InSessionId),
		  ModIds(InIds)
	{}

public:
	friend struct Modio::MetricsSessionParams ToModio(const FModioMetricsSessionParams& In);

	/** @brief Set a custom Session Id to be used in the metrics service for your session */
	TOptional<FModioGuid> SessionId;

	/** @brief Includes mods which will be tracked as part of the metrics service */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "mod.io|CommonTypes")
	TArray<FModioModID> ModIds;
};

/**
 * @docpublic
 * @brief Enumerator with the possible memory measurement units
 */
UENUM(BlueprintType)
enum EFileSizeUnit
{
	/** Will take the largest one that becomes a number larger than 1 (i.e, 1300mb becomes 1.3gb) */
	Largest = 0,
	/** A single byte */
	B = 1,
	/** Kilobytes */
	KB = 1024,
	/** Megabytes */
	MB = 1024 * 1024,
	/** Gigabytes */
	GB = 1024 * 1024 * 1024
};

/**
 * @docpublic
 * @brief Strong type struct to wrap Token Pack ID
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioTokenPackID
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioTokenPackID();

	/**
	 * @docpublic
	 * @brief Preferred constructor with Token Pack initialization parameter
	 * @param InTokenPackId Base Token Pack ID to create this strong type
	 */
	FModioTokenPackID(FString InTokenPackId)
	{
		TokenPackID = InTokenPackId;
	}

	/**
	 * @docpublic
	 * @brief Transform a TokenPackID into its 32 bit integer representation
	 * @param ModioTokenPackId String type for the TokenPackID to read its hash from
	 * @return unsigned 32 bit integer that matches the hash of the TokenPackID provided
	 */
	MODIO_API friend uint32 GetTypeHash(FModioTokenPackID ModioTokenPackId);

	/**
	 * @docpublic
	 * @brief Comparison operator between TokenPackID elements
	 */
	MODIO_API friend bool operator==(FModioTokenPackID A, FModioTokenPackID B)
	{
		return A.TokenPackID == B.TokenPackID;
	}

	/**
	 * @docpublic
	 * @brief Comparison operator between TokenPackID elements
	 */
	MODIO_API friend bool operator!=(FModioTokenPackID A, FModioTokenPackID B)
	{
		return A.TokenPackID != B.TokenPackID;
	}

	/**
	 * @docpublic
	 * @brief Transform a TokenPackID into its string representation
	 * @return String value of the stored TokenPackID
	 */
	FString ToString() const
	{
		return TokenPackID;
	}

	/**
	 * @docpublic
	 * @brief Stream forward operator to pass the TokenPackID along
	 * @param Ar The archive class that receives information
	 * @param ID The TokenPackID to pass along this operator
	 * @return FArchive The updated archive with the TokenPackID passed along
	 */
	MODIO_API friend FArchive& operator<<(FArchive& Ar, FModioTokenPackID& ID)
	{
		return Ar << ID.TokenPackID;
	}

	/**
	 * @docpublic
	 * @brief Store this instance TokenPackID into an archive
	 * @param Ar The archive class that receives information
	 * @return Always true when the TokenPackID is forwarded to Ar
	 */
	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	/**
	 * @docpublic
	 * @brief Store this instance TokenPackID into an archive with reference flag as parameter
	 * @param Ar The archive class that receives information
	 * @param Map A dictionary from indices for network communication
	 * @param bOutSuccess Flag to signal the result of this operation, True when stored
	 * @return Always true when the ModID is forwarded to Ar
	 */
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << *this;
		bOutSuccess = true;
		return true;
	}

private:
	/**
	 * @docinternal
	 * @brief Function for retrieving the underlying value of an FModioTokenPackID for use in custom serialization.
	 * Not recommended for any other use. FModioTokenPackIDs should be treated as opaque.
	 * @param In the FModioTokenPackID to retrieve the value for
	 * @return the underlying string ID.
	 */
	MODIO_API friend FString GetUnderlyingValue(const FModioTokenPackID& In)
	{
		return In.TokenPackID;
	}

	FString TokenPackID;
};
