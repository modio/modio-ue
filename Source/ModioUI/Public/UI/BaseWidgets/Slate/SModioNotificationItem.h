#pragma once

#include "Misc/EngineVersionComparison.h"
#include "Widgets/Notifications/SNotificationList.h"

class MODIOUI_API SModioNotificationItem : public SNotificationItem
{
protected:
	float ExpireDuration;
	TSharedPtr<class SModioNotificationList> MyList;

	virtual EActiveTimerReturnType HandleExpire(double, float);

public:
	SLATE_BEGIN_ARGS(SModioNotificationItem) {}
	SLATE_DEFAULT_SLOT(FArguments, Content);
	SLATE_END_ARGS()

	void Construct(const FArguments& Args)
	{
		ChildSlot.AttachWidget(Args._Content.Widget);
	};

	void SetList(TSharedRef<SModioNotificationList> List);
	void SetText(const TAttribute<FText>& InText) override;

	void SetHyperlink(const FSimpleDelegate& InHyperlink,
					  const TAttribute<FText>& InHyperlinkText = TAttribute<FText>()) override;

	void SetExpireDuration(float InExpireDuration) override;

	void SetFadeInDuration(float FadeInDuration) override;

	void SetFadeOutDuration(float FadeOutDuration) override;

	ECompletionState GetCompletionState() const override;

	void SetCompletionState(ECompletionState State) override;

	void ExpireAndFadeout() override;

	void Fadeout() override;

	void Pulse(const FLinearColor& GlowColor) override;
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	virtual void SetSubText(const TAttribute<FText>& InSubText) override;
#else
	virtual void SetSubText(const TAttribute<FText>& InSubText);
#endif
	
};