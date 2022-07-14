#include "UI/Styles/ModioUIBrushRef.h"
#include "UI/Styles/ModioUIStyleSet.h"

FName FModioUIMaterialRef::GetMaterialName()
{
	return MaterialName;
}

UMaterialInterface* FModioUIMaterialRef::ResolveReference(UModioUIStyleSet* OverrideStyleSet,
														  FName AdditionalSpecifier) const
{
	if (OverrideStyleSet)
	{
		return OverrideStyleSet->GetNamedMaterial(
			MaterialName, AdditionalSpecifier != NAME_None ? AdditionalSpecifier.ToString() : TOptional<FString> {});
	}

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			return DefaultStyleSet->GetNamedMaterial(MaterialName, AdditionalSpecifier != NAME_None
																	   ? AdditionalSpecifier.ToString()
																	   : TOptional<FString> {});
		}
	}

	return nullptr;
}
