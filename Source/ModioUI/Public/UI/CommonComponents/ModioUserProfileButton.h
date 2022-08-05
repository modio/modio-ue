// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioCircularProgressBar.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"
#include "UI/EventHandlers/IModioUIUserAvatarDownloadCompletedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioUserProfileButton.generated.h"

/**
 *
 */
// Needs to know about login/logout events
UCLASS()
class MODIOUI_API UModioUserProfileButton : public UModioModManagementWidgetBase,
											public IModioUIUserChangedReceiver,
											public IModioUIUserAvatarDownloadCompletedReceiver
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	virtual void OnButtonClicked();
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void UpdateProgress(const struct FModioModProgressInfo& ProgressInfo) override;
	TOptional<FModioUser> CurrentUser;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioCircularProgressBar* DownloadProgressIndicator;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* ProfileButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioImage* ProfileImage;

	// To stop the material being GC'd while in use
	UPROPERTY(Transient)
	UMaterialInterface* GCMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSoftObjectPtr<UMaterialInterface> UserBrushMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TSoftObjectPtr<UMaterialInterface> NoUserBrushMaterial;

	UPROPERTY(EditAnywhere, Category="Widgets")
	FName ProfileTextureParameterName = "WidgetTexture";

	void NativeDestruct() override;

	void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ec, TOptional<FModioImageWrapper> Image) override;

public:
	// TODO: @modio-ue4 make this private again and add a function to the UserChangedReceiver interface to allow setting
	// the user directly?
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
};
