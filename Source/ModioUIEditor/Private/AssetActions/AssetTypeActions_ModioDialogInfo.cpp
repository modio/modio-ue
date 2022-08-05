#include "AssetActions/AssetTypeActions_ModioDialogInfo.h"
#include "Editor/ModioDialogInfoEditorToolkit.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "UI/Dialogs/ModioDialogInfo.h"

UClass* FAssetTypeActions_ModioDialogInfo::GetSupportedClass() const
{
	return UModioDialogInfo::StaticClass();
}

uint32 FAssetTypeActions_ModioDialogInfo::GetCategories()
{
	return EAssetTypeCategories::UI;
}

void FAssetTypeActions_ModioDialogInfo::OpenAssetEditor(
	const TArray<UObject*>& InObjects,
	TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	TSharedRef<FModioDialogEditorToolkit> DialogEditor(new FModioDialogEditorToolkit());
	DialogEditor->InitAssetEditor(Mode, EditWithinLevelEditor, InObjects[0]);
}
