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

class MODIOUI_API SModioImageGallery : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModioImageGallery) {}
	// delegate for getting the widget for each image

	SLATE_END_ARGS()
};