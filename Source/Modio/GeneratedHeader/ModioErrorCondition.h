/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io SDK.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *  
 */

#pragma once

#include "ModioErrorCondition.generated.h"

UENUM(BlueprintType)
enum class EModioErrorCondition: uint8
{
	NoError = 0,
	NetworkError = 2 UMETA(ToolTip="When this condition is true, the error code represents a connection or HTTP error between the client and the mod.io server.") ,
	ConfigurationError = 3 UMETA(ToolTip="When this condition is true, the error code indicates the SDK's configuration is not valid - the game ID or API key are incorrect or the game has been deleted.") ,
	InvalidArgsError = 4 UMETA(ToolTip="When this condition is true, the error code indicates the arguments passed to the function have failed validation or were otherwise invalid.") ,
	FilesystemError = 5 UMETA(ToolTip="When this condition is true, the error code indicates a permission or IO error when accessing local filesystem data.") ,
	InternalError = 6 UMETA(ToolTip="When this condition is true, the error code represents an internal SDK error - please inform mod.io of the error code value.") ,
	EntityNotFoundError = 12 UMETA(ToolTip="When this condition is true, the error code indicates that a specified game, mod, media file or mod file was not found.") ,
	UserTermsOfUseError = 13 UMETA(ToolTip="When this condition is true, the error code indicates that the user has not yet accepted the mod.io Terms of Use.") ,
	SubmitReportError = 14 UMETA(ToolTip="When this condition is true, the error code indicates that a report for the specified content could not be submitted.") ,
};
