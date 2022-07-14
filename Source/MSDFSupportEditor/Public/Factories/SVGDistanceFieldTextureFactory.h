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

#include "CoreMinimal.h"
#include "EditorReimportHandler.h"
#include "Engine/Texture2DDynamic.h"
#include "Factories/Factory.h"

#include "SVGDistanceFieldTextureFactory.generated.h"

enum class EImportSVGDialogReturnType : uint8;
/**
 *
 */
UCLASS()
class MSDFSUPPORTEDITOR_API USVGDistanceFieldTextureFactory : public UFactory, public FReimportHandler
{
	GENERATED_BODY()

	// Begin FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	// End FReimportHandler interface

	EImportSVGDialogReturnType Result;
	void OnImportOptionsDialogClosed(EImportSVGDialogReturnType DialogResult);
	void OnUpdatePreview();
	TSharedPtr<SWindow> OptionsWindow;
	TSharedPtr<struct FSVGConversionParams> Params;
	void UpdateTextureData(class UTexture2DDynamic* TargetTexture, const TArray<uint8>& SourceData);
	UPROPERTY()
	UTexture2DDynamic* SDFTexture;
	UPROPERTY()
	UTexture2DDynamic* RenderedTexture;
	uint32 OutputTextureSize = 256;
	virtual UClass* ResolveSupportedClass() override;
	bool UpdatePreviewTextures();
	FString CurrentFileName;
	void UpdateTextureParameters(UTexture2D* SVGTextureAsset);

public:
	USVGDistanceFieldTextureFactory();

	virtual bool ShouldShowInNewMenu() const override;

	virtual uint32 GetMenuCategories() const override;

	virtual bool FactoryCanImport(const FString& Filename) override;

	UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
							   const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn,
							   bool& bOutOperationCanceled) override;

	
};
