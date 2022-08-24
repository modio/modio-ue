/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Factories/SVGDistanceFieldTextureFactory.h"
#include "AssetRegistryModule.h"
#include "AssetTypeCategories.h"
#include "EditorFramework/AssetImportData.h"
#include "HAL/ThreadManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "MSDFAssetData.h"
#include "Misc/EngineVersionComparison.h"
#include "Misc/FeedbackContext.h"
#include "Misc/ScopedSlowTask.h"
#include "SVGToSDF.h"
#include "Slate/SImportSVGOptionsWidget.h"
#include "UObject/SavePackage.h"

bool USVGDistanceFieldTextureFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	if (UTexture2D* ConcreteAsset = Cast<UTexture2D>(Obj))
	{
		if (UMSDFAssetData* AssetData = ConcreteAsset->GetAssetUserData<UMSDFAssetData>())
		{
			TArray<FString> AssetFileNames = ConcreteAsset->AssetImportData->ExtractFilenames();
			if (AssetFileNames.Num())
			{
				OutFilenames.Add(
					UAssetImportData::ResolveImportFilename(AssetFileNames[0], ConcreteAsset->GetOutermost()));
				return true;
			}
		}
	}
	return false;
}

void USVGDistanceFieldTextureFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (UTexture2D* ConcreteAsset = Cast<UTexture2D>(Obj))
	{
		if (NewReimportPaths.Num())
		{
			ConcreteAsset->AssetImportData->AddFileName(
				UAssetImportData::SanitizeImportFilename(NewReimportPaths[0], ConcreteAsset->GetOutermost()), 0);
		}
	}
}

EReimportResult::Type USVGDistanceFieldTextureFactory::Reimport(UObject* Obj)
{
	if (UTexture2D* ConcreteAsset = Cast<UTexture2D>(Obj))
	{
		TArray<uint8> OutputData;
		FString Filename;
		if (UMSDFAssetData* AssetData = ConcreteAsset->GetAssetUserData<UMSDFAssetData>())
		{
			Params = MakeShared<FSVGConversionParams>(AssetData->ImportParameters);
			TArray<FString> AssetFileNames = ConcreteAsset->AssetImportData->ExtractFilenames();
			if (AssetFileNames.Num())
			{
				Filename = UAssetImportData::ResolveImportFilename(AssetFileNames[0], ConcreteAsset->GetOutermost());
				if (!Filename.IsEmpty() &&
					USVGToSDFFunctionLibrary::ConvertSVGToMSDFData(Filename, AssetData->ImportParameters, OutputData))
				{
					ConcreteAsset->Source.Init(Params->OutputDimensions.X, Params->OutputDimensions.Y, 1, 1,
											   ETextureSourceFormat::TSF_BGRA8, OutputData.GetData());
					UpdateTextureParameters(ConcreteAsset);
					ConcreteAsset->Modify(true);
					return EReimportResult::Succeeded;
				}
			}
		}
	}
	return EReimportResult::Failed;
}

void USVGDistanceFieldTextureFactory::OnImportOptionsDialogClosed(EImportSVGDialogReturnType DialogResult)
{
	Result = DialogResult;
	if (OptionsWindow)
	{
		OptionsWindow->RequestDestroyWindow();
	}
}

void USVGDistanceFieldTextureFactory::OnUpdatePreview()
{
	UpdatePreviewTextures();
}

USVGDistanceFieldTextureFactory::USVGDistanceFieldTextureFactory()
{
	SupportedClass = nullptr;
	bEditAfterNew = true;
	bEditorImport = true;
	Formats.Add(TEXT("svg;SVG Image"));
	ImportPriority = DefaultImportPriority + 1;
	SDFTexture = UTexture2DDynamic::Create(32, 32, FTexture2DDynamicCreateInfo(PF_B8G8R8A8));
	RenderedTexture = UTexture2DDynamic::Create(32, 32, FTexture2DDynamicCreateInfo(PF_B8G8R8A8));
}

bool USVGDistanceFieldTextureFactory::ShouldShowInNewMenu() const
{
	// We don't create new textures, we import existing SVGs
	return false;
}

uint32 USVGDistanceFieldTextureFactory::GetMenuCategories() const
{
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	return EAssetTypeCategories::Textures;
#else
	return EAssetTypeCategories::MaterialsAndTextures;
#endif
}

bool USVGDistanceFieldTextureFactory::FactoryCanImport(const FString& Filename)
{
	// We only support SVG files
	if (FPaths::GetExtension(Filename) != TEXT("svg"))
	{
		return false;
	}
	return true;
}

UObject* USVGDistanceFieldTextureFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName,
															EObjectFlags Flags, const FString& Filename,
															const TCHAR* Parms, FFeedbackContext* Warn,
															bool& bOutOperationCanceled)
{
	CurrentFileName = Filename;
	InName = FName("T_MSDF_" + InName.ToString());
	Params = MakeShared<FSVGConversionParams>();

	UpdatePreviewTextures();
	// Create a new window containing our import options widgets and then show it as a modial dialog
	OptionsWindow = SNew(SWindow).Title(FText::FromString("MSDF Import Options")).ClientSize(FVector2D(800, 310));

	OptionsWindow->SetContent(
		SNew(SImportSVGOptionsWidget)
			.OptionsToEdit(Params)
			.SDFPreviewTexture(SDFTexture)
			.RenderedTexture(RenderedTexture)
			.OnDialogClosed_UObject(this, &USVGDistanceFieldTextureFactory::OnImportOptionsDialogClosed)
			.OnUpdatePreview_UObject(this, &USVGDistanceFieldTextureFactory::OnUpdatePreview));

	TSharedPtr<SWindow> ParentWindow;

	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}
	Result = EImportSVGDialogReturnType::Open;

	FSlateApplication::Get().AddModalWindow(OptionsWindow.ToSharedRef(), ParentWindow, false);

	// Ensure that the dialog indicated the user wants to continue
	if (Result != EImportSVGDialogReturnType::Confirmed)
	{
		bOutOperationCanceled = true;
		return nullptr;
	}
	FText TaskMessage = FText::FromString("Generating MSDF data...");
	FScopedSlowTask ImportTask(3, TaskMessage);
	ImportTask.MakeDialog(false, false);
	ImportTask.EnterProgressFrame(1, TaskMessage);
	FString ImportedFileName = FPaths::GetCleanFilename(Filename);
	ImportedFileName.ReplaceCharInline('.', '_');
	ImportedFileName = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / ImportedFileName;

	// We want to make sure we create packages and objects with a name matching our naming conventions rather than the
	// source file name, so create the right name
	FString InPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / InName.ToString();
	UPackage* InPackage = CreatePackage(*InPackagePath); // Create the actual package we want to save to

	UTexture2D* SVGTextureAsset = NewObject<UTexture2D>(InPackage, InClass, InName, Flags);

	// apply the settings here

	TArray<uint8> OutputData;
	if (!USVGToSDFFunctionLibrary::ConvertSVGToMSDFData(Filename, *Params.Get(), OutputData))
	{
		Warn->Log(ELogVerbosity::Error, FString("Could not convert SVG to MSDF texture data!"));
		bOutOperationCanceled = true;
		return nullptr;
	}

	TaskMessage = FText::FromString("Updating Texture...");
	ImportTask.EnterProgressFrame(1, TaskMessage);
	SVGTextureAsset->Source.Init(Params->OutputDimensions.X, Params->OutputDimensions.Y, 1, 1,
								 ETextureSourceFormat::TSF_BGRA8, OutputData.GetData());

	UpdateTextureParameters(SVGTextureAsset);

	TaskMessage = FText::FromString("Saving asset...");
	ImportTask.EnterProgressFrame(1, TaskMessage);
	if (SVGTextureAsset->AssetImportData)
	{
		SVGTextureAsset->AssetImportData->AddFileName(
			UAssetImportData::SanitizeImportFilename(Filename, SVGTextureAsset->GetOutermost()), 0);
	}
	UMSDFAssetData* ImportParameters = NewObject<UMSDFAssetData>(SVGTextureAsset);
	ImportParameters->ImportParameters = *Params.Get();
	SVGTextureAsset->AddAssetUserData(ImportParameters);

	InPackagePath = InPackagePath + FPackageName::GetAssetPackageExtension(); //
	if (!FPackageName::TryConvertGameRelativePackagePathToLocalPath(InPackagePath, InPackagePath))
	{
		Warn->Log(ELogVerbosity::Error, "Could not convert resolve package path: " + InPackagePath);
		bOutOperationCanceled = true;
		return nullptr;
	}
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	FSavePackageArgs Args;
	Args.TopLevelFlags = RF_Standalone | RF_Public;
	UPackage::SavePackage(InPackage, SVGTextureAsset, *InPackagePath, Args);
#else
	UPackage::SavePackage(InPackage, SVGTextureAsset, RF_Standalone | RF_Public, *InPackagePath);
#endif

	FAssetRegistryModule::AssetCreated(SVGTextureAsset);

	return SVGTextureAsset;
}

void USVGDistanceFieldTextureFactory::UpdateTextureParameters(UTexture2D* SVGTextureAsset)
{
	FRenderCommandFence InitializationFence;
	SVGTextureAsset->Filter = TF_Default;
#if WITH_EDITORONLY_DATA
	SVGTextureAsset->CompressionNone = true;
	SVGTextureAsset->DeferCompression = false;
#endif // #if WITH_EDITORONLY_DATA

	SVGTextureAsset->bNoTiling = true;
	SVGTextureAsset->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
	SVGTextureAsset->SRGB = false;
	SVGTextureAsset->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	SVGTextureAsset->AlphaCoverageThresholds = FVector4(0, 0, 0, 1);
	SVGTextureAsset->AddressX = TA_Clamp;
	SVGTextureAsset->AddressY = TA_Clamp;
	SVGTextureAsset->UpdateResource();

	InitializationFence.BeginFence();
	InitializationFence.Wait();
	SVGTextureAsset->PostEditChange();
}

void USVGDistanceFieldTextureFactory::UpdateTextureData(UTexture2DDynamic* TargetTexture,
														const TArray<uint8>& SourceData)
{
	FRenderCommandFence InitializationFence;
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	FTexture2DDynamicResource* TextureResource = static_cast<FTexture2DDynamicResource*>(TargetTexture->GetResource());
#else
	FTexture2DDynamicResource* TextureResource = static_cast<FTexture2DDynamicResource*>(TargetTexture->Resource);
#endif
	bool bDone = false;
	ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)
	([&bDone, TextureResource, SourceData](FRHICommandListImmediate& RHICmdList) {
		check(IsInRenderingThread());

		// Using auto keyword as the type is different after 4.25 but is used in the same manner
		auto TextureRHI = TextureResource->GetTexture2DRHI();

		int32 Width = TextureRHI->GetSizeX();
		int32 Height = TextureRHI->GetSizeY();

		uint32 DestStride = 0;
		uint8* DestData =
			reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));
		FMemory::Memcpy(DestData, SourceData.GetData(), Height * DestStride);

		RHIUnlockTexture2D(TextureRHI, 0, false, false);
		bDone = true;
	});
	InitializationFence.BeginFence();
	InitializationFence.Wait();
}

UClass* USVGDistanceFieldTextureFactory::ResolveSupportedClass()
{
	return UTexture2D::StaticClass();
}

bool USVGDistanceFieldTextureFactory::UpdatePreviewTextures()
{
	TArray<uint8> OutputData;
	if (!USVGToSDFFunctionLibrary::ConvertSVGToMSDFData(CurrentFileName, *Params.Get(), OutputData))
	{
		return false;
	}
	else
	{
		SDFTexture->SizeX = Params->OutputDimensions.X;
		SDFTexture->SizeY = Params->OutputDimensions.Y;
		SDFTexture->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
		SDFTexture->SRGB = false;
		SDFTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		SDFTexture->UpdateResource();
		FRenderCommandFence SizeFence;
		SizeFence.BeginFence();
		SizeFence.Wait();
		UpdateTextureData(SDFTexture, OutputData);
		return true;
	}
}
