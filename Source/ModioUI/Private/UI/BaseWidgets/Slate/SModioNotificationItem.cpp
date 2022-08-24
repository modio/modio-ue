#include "UI/BaseWidgets/Slate/SModioNotificationItem.h"
#include "UI/BaseWidgets/Slate/SModioNotificationList.h"

void SModioNotificationItem::SetList(TSharedRef<SModioNotificationList> List)
{
	MyList = List;
}

void SModioNotificationItem::SetText(const TAttribute<FText>& InText)
{
	
}

void SModioNotificationItem::SetHyperlink(const FSimpleDelegate& InHyperlink,
										  const TAttribute<FText>& InHyperlinkText /*= TAttribute<FText>()*/)
{}

void SModioNotificationItem::SetExpireDuration(float InExpireDuration)
{
	ExpireDuration = InExpireDuration;
}

void SModioNotificationItem::SetFadeInDuration(float FadeInDuration) {}

void SModioNotificationItem::SetFadeOutDuration(float FadeOutDuration) {}

SNotificationItem::ECompletionState SModioNotificationItem::GetCompletionState() const
{
	return SNotificationItem::CS_None;
}

void SModioNotificationItem::SetCompletionState(ECompletionState State) {}

void SModioNotificationItem::ExpireAndFadeout()
{
	RegisterActiveTimer(ExpireDuration,
						FWidgetActiveTimerDelegate::CreateSP(this, &SModioNotificationItem::HandleExpire));
}

EActiveTimerReturnType SModioNotificationItem::HandleExpire(double, float)
{
	if (MyList)
	{
		MyList->NotificationItemFadedOut(SharedThis<SModioNotificationItem>(this));
	}
	return EActiveTimerReturnType::Stop;
}
void SModioNotificationItem::Fadeout() {}

void SModioNotificationItem::Pulse(const FLinearColor& GlowColor) {}

void SModioNotificationItem::SetSubText(const TAttribute<FText>& InSubText) {}
