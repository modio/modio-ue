#include "UI/BaseWidgets/ModioUIAsyncLoadingOverlay.h"

#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/Styles/ModioDialogStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SBox.h"

void UModioUIAsyncLoadingOverlay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UModioUIAsyncLoadingOverlay::Close);
	}
}


TSharedRef<SWidget> UModioUIAsyncLoadingOverlay::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	
	return Widget;
}

void UModioUIAsyncLoadingOverlay::Close()
{
	OwningDialog->HideLoadingSpinner();
}

void UModioUIAsyncLoadingOverlay::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	ApplyStyling();
}

void UModioUIAsyncLoadingOverlay::ApplyStyling()
{
	const FModioDialogStyle* ResolvedStyle = DialogStyle.FindStyle<FModioDialogStyle>();
	if (ResolvedStyle)
	{
		if (BackgroundImage)
		{
			BackgroundImage->SetBrush(ResolvedStyle->BackgroundBrush);
		}
	}
}
