/*
 *  Copyright (C) 2022 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Misc/EngineVersionComparison.h"

namespace MaterialExpressionHelpers
{
	inline void AddMaterialExpression(UMaterial* Target, UMaterialExpression* Expression)
	{
#if UE_VERSION_OLDER_THAN(5, 1, 0)
		Target->Expressions.Add(Expression);
#else
		Target->GetExpressionCollection().AddExpression(Expression);
#endif
	}
	inline void ConnectEmissive(UMaterial* Target, UMaterialExpression* Expression)
	{
#if UE_VERSION_OLDER_THAN(5, 1, 0)
		Target->EmissiveColor.Connect(0, Expression);
#else
		Target->GetEditorOnlyData()->EmissiveColor.Connect(0, Expression);
#endif
	}
	inline void ConnectOpacity(UMaterial* Target, UMaterialExpression* Expression)
	{
#if UE_VERSION_OLDER_THAN(5, 1, 0)
		Target->Opacity.Connect(0, Expression);
#else
		Target->GetEditorOnlyData()->Opacity.Connect(0, Expression);
#endif
	}
} // namespace MaterialExpressionHelpers
