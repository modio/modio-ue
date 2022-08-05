#pragma once

#include "Components/NamedSlotInterface.h"
#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "ModioUserWidgetBase.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "Containers/Array.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Images/SImage.h"

#include "ModioUIAsyncLoadingOverlay.generated.h"


UCLASS()
class UModioUIAsyncLoadingOverlay : public UModioUserWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetOwningDialog(UModioDialogBaseInternal* Dialog)
	{
		OwningDialog = Dialog;
	}

	void NativeOnInitialized() override;

private:
	UModioDialogBaseInternal* OwningDialog;
	
protected:
	UFUNCTION()
	void Close();
	TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;

	void ApplyStyling();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioDialogStyle"), Category="Widgets")
	FModioUIStyleRef DialogStyle = {FName("DefaultDialogStyle")};

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* CancelButton;
	
	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* LoadingText;

	UPROPERTY(BlueprintReadOnly,Category="Widgets", meta = (BindWidget))
	UImage* BackgroundImage;
};
