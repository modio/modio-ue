/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "Tests/Commands/ModioTestCommandBase.h"
#include "Types/ModioCommonTypes.h"

#if WITH_DEV_AUTOMATION_TESTS

class MODIOTESTS_API FModioGetModIdsByNameCommand : public FModioTestLatentCommandBase
{
public:
	FModioGetModIdsByNameCommand(FAutomationTestBase* InTestBase, const TArray<FString>& InModNames,
								 TSharedPtr<TArray<FModioModID>> InResult)
		: FModioTestLatentCommandBase(InTestBase),
		  ModNames(InModNames),
		  Result(InResult)
	{}

	virtual bool ModioUpdate(float DeltaTime) override;
	virtual void Start() override;
	bool RequestModIds();
	bool CheckModIdsReceived();

	TArray<FString> ModNames;
	TSharedPtr<TArray<FModioModID>> Result;
	bool bRequested = false;
};

#endif