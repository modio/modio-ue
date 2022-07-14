#include "UI/Commands/ModioCommonUICommands.h"
#include "Framework/Commands/UICommandInfo.h"

#include "Loc/BeginModioLocNamespace.h"

FModioUIMenuEntry UModioUICommandInfoLibrary::PreviousPageCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().PreviousPage->GetLabel()};
}

FModioUIMenuEntry UModioUICommandInfoLibrary::NextPageCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().NextPage->GetLabel()};
}

FModioUIMenuEntry UModioUICommandInfoLibrary::MoreOptionsCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().MoreOptions->GetLabel()};
}

FModioUIMenuEntry UModioUICommandInfoLibrary::ConfirmCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().Confirm->GetLabel()};
}

FModioUIMenuEntry UModioUICommandInfoLibrary::RefineSearchCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().RefineSearch->GetLabel()};
}

FModioUIMenuEntry UModioUICommandInfoLibrary::BackCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().Back->GetLabel()};
}

FModioUIMenuEntry UModioUICommandInfoLibrary::DownloadQueueCommand()
{
	return FModioUIMenuEntry {FModioCommonUICommands::Get().DownloadQueue->GetLabel()};
}

FUIAction FModioCommonUICommands::GetNullAction()
{
	return FUIAction(FExecuteAction::CreateLambda([]() {}), FCanExecuteAction::CreateLambda([]() { return false; }));
}

void FModioCommonUICommands::RegisterCommands()
{
	// The input chords passed here need to map to our custom keys
	// The input settings for the plugin needs to map an arbitrary number of input chords to the custom key

	FUICommandInfo::MakeCommandInfo(this->AsShared(), PreviousPage, "Previous", LOCTEXT("Previous.Label", "Prev"),
									LOCTEXT("Previous.Desc", "Displays the previous page"), FSlateIcon(),
									EUserInterfaceActionType::Button,
									FInputChord(FModioInputKeys::Previous, EModifierKey::None));

	FUICommandInfo::MakeCommandInfo(this->AsShared(), NextPage, "Next", LOCTEXT("Next.Label", "Next"),
									LOCTEXT("Next.Desc", "Displays the next page"), FSlateIcon(),
									EUserInterfaceActionType::Button,
									FInputChord(FModioInputKeys::Next, EModifierKey::None));

	FUICommandInfo::MakeCommandInfo(
		this->AsShared(), MoreOptions, "MoreOptions", LOCTEXT("MoreOptions.Label", "More Options"),
		LOCTEXT("MoreOptions.Desc", "Displays additional options for the current item"), FSlateIcon(),
		EUserInterfaceActionType::Button, FInputChord(FModioInputKeys::MoreOptions, EModifierKey::None));

	FUICommandInfo::MakeCommandInfo(
		this->AsShared(), Confirm, "Confirm", LOCTEXT("Confirm.Label", "Confirm"),
		LOCTEXT("Confirm.Desc", "Confirms or performs the default action for the current item"), FSlateIcon(),
		EUserInterfaceActionType::Button, FInputChord(FModioInputKeys::Confirm, EModifierKey::None));
	FUICommandInfo::MakeCommandInfo(
		this->AsShared(), Subscribe, "Subscribe", LOCTEXT("Subscribe.Label", "Subscribe"),
		LOCTEXT("Subscribe.Desc", "Triggers a subscription or unsubscription for the current context"), FSlateIcon(),
		EUserInterfaceActionType::Button, FInputChord(FModioInputKeys::Subscribe, EModifierKey::None));

	FUICommandInfo::MakeCommandInfo(
		this->AsShared(), RefineSearch, "RefineSearch", LOCTEXT("RefineSearch.Label", "Refine Search"),
		LOCTEXT("Refine Search.Desc", "Filters the current set of results"), FSlateIcon(),
		EUserInterfaceActionType::Button, FInputChord(FModioInputKeys::RefineSearch, EModifierKey::None));

	FUICommandInfo::MakeCommandInfo(this->AsShared(), Back, "Back", LOCTEXT("Back.Label", "Back"),
									LOCTEXT("Back.Desc", "Returns to the previous screen or exits the browser"),
									FSlateIcon(), EUserInterfaceActionType::Button,
									FInputChord(FModioInputKeys::Back, EModifierKey::None));
	FUICommandInfo::MakeCommandInfo(
		this->AsShared(), DownloadQueue, "DownloadQueue", LOCTEXT("DownloadQueue.Label", "Download Queue"),
		LOCTEXT("DownloadQueue.Desc", "Displays the user profile and download queue"), FSlateIcon(),
		EUserInterfaceActionType::Button, FInputChord(FModioInputKeys::DownloadQueue, EModifierKey::None));
}

#include "Loc/EndModioLocNamespace.h"