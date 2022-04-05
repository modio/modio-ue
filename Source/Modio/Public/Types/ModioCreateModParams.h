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

#include "ModioCreateModParams.generated.h"

USTRUCT(BlueprintType)
struct MODIO_API FModioCreateModParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString PathToLogoFile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString Summary;

	TOptional<FString> NamePath;

	TOptional<bool> bVisible;
	
	TOptional<FString> Description;
	
	TOptional<FString> HomepageURL;
	
	TOptional<int64> QuantityAvailable;
	
	TOptional<uint8> MaturityFlags;
	
	TOptional<FString> MetadataBlob;
	
	TOptional<TArray<FString>> Tags; 

};