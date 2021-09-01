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

#include "ModioSDK.h"
#include "Types/ModioTerms.h"
#include "Internal/ModioConvert.h"

FORCEINLINE FModioLink ToUnreal(const Modio::Terms::Link& In) 
{
	FModioLink Out;
	Out.Text = ToUnreal(In.Text);
	Out.URL = ToUnreal(In.URL);
	Out.bRequired = ToUnreal(In.bRequired);
	return Out;
}

FORCEINLINE FModioTerms ToUnreal(const Modio::Terms& In) 
{
	FModioTerms Out;
	Out.AgreeButtonText = ToUnreal(In.Buttons.AgreeText);
	Out.DisagreeButtonText = ToUnreal(In.Buttons.DisagreeText);
	Out.WebsiteLink = ToUnreal(In.Links.Website);
	Out.TermsLink = ToUnreal(In.Links.Terms);
	Out.PrivacyLink = ToUnreal(In.Links.Privacy);
	Out.ManageLink = ToUnreal(In.Links.Manage);
	Out.TermsText = ToUnreal(In.TermsText);
	return Out;
}