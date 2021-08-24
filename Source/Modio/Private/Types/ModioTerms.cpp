#include "Types/ModioTerms.h"

#include "ModioSDK.h"
#include "Internal/ModioConvert.h"

FModioLink::FModioLink(const Modio::Terms::Link& Link) :
	Text(ToUnreal(Link.Text)),
	URL(ToUnreal(Link.URL)),
	bRequired(ToUnreal(Link.bRequired))
{
}

FModioTerms::FModioTerms(const Modio::Terms& Terms) :
	AgreeButtonText(ToUnreal(Terms.Buttons.AgreeText)),
	DisagreeButtonText(ToUnreal(Terms.Buttons.DisagreeText)),
	WebsiteLink(ToUnreal(Terms.Links.Website)),
	TermsLink(ToUnreal(Terms.Links.Terms)),
	PrivacyLink(ToUnreal(Terms.Links.Privacy)),
	ManageLink(ToUnreal(Terms.Links.Manage)),
	TermsText(ToUnreal(Terms.TermsText))
{
}

FModioOptionalTerms::FModioOptionalTerms(TOptional<FModioTerms>&& ModTagOptions) :
	Internal(MoveTemp(ModTagOptions))
{
}