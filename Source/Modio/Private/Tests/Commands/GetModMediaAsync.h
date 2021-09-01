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

#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FGetModLogoCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;
	EModioLogoSize Size;

public:
	FGetModLogoCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, EModioLogoSize Size)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID),
		  Size(Size)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->GetModMediaAsync(ModID, Size, FOnGetMediaDelegateFast::CreateSP(this, &FGetModLogoCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioImage> ModInfo)
	{
		CurrentTest->TestEqual("Mod Logo retrieval completed with expected result", ec, false);
		Done();
	}
};

class FGetModGalleryImageCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;
	EModioGallerySize Size;
	uint32 Index;
public:
	FGetModGalleryImageCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, uint32 Index,
							   EModioGallerySize Size)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID),
		  Size(Size),
		  Index(Index)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->GetModMediaAsync(ModID, Size, Index,
								FOnGetMediaDelegateFast::CreateSP(this, &FGetModGalleryImageCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioImage> ModInfo)
	{
		CurrentTest->TestEqual("Mod Logo retrieval completed with expected result", ec, false);
		Done();
	}
};

class FGetModCreatorAvatarCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;
	EModioAvatarSize Size;

public:
	FGetModCreatorAvatarCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, EModioAvatarSize Size)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID),
		  Size(Size)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->GetModMediaAsync(ModID, Size,
								FOnGetMediaDelegateFast::CreateSP(this, &FGetModCreatorAvatarCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioImage> ModInfo)
	{
		CurrentTest->TestEqual("Mod Logo retrieval completed with expected result", ec, false);
		Done();
	}
};
#endif