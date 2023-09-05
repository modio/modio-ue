/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IModioExtendedModInfoUIDetails.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioExtendedModInfoUIDetails : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUICORE_API IModioExtendedModInfoUIDetails
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool IsModSubscribed() const;
	virtual bool IsModSubscribed_Implementation() const
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool IsModDownloading() const;
	virtual bool IsModDownloading_Implementation() const
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool IsModExtracting() const;
	virtual bool IsModExtracting_Implementation() const
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool IsModInQueue() const;
	virtual bool IsModInQueue_Implementation() const
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool IsModEnabled();
	virtual bool IsModEnabled_Implementation()
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool IsModInstalled() const;
	virtual bool IsModInstalled_Implementation() const
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IIModioExtendedModInfoUIDetails")
	bool HasModErrors() const;
	virtual bool HasModErrors_Implementation() const
	{
		return false;
	}
};
