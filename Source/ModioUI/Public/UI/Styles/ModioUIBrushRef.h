#pragma once
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UObject/NameTypes.h"

#include "ModioUIBrushRef.generated.h"


USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIMaterialRef
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FName MaterialName;

	friend class FModioSlateBrushStructCustomization;

public:
	FModioUIMaterialRef() : MaterialName(NAME_None) {};
	FModioUIMaterialRef(FName MaterialName) : MaterialName(MaterialName) {};

	void SetMaterialName(FName NewName)
	{
		MaterialName = NewName;
	}
	FName GetMaterialName();
	/// @param OverrideStyleSet By default the reference will be resolved against the default style set, this provides a
	/// different styleset to reference. Not intended for normal use, currently used only to break a circular dependency
	/// when setting a new UModioUIStyleSet.
	UMaterialInterface* ResolveReference(UModioUIStyleSet* OverrideStyleSet = nullptr, FName AdditionalSpecifier = NAME_None) const;
};

/*
template<>
struct TStructOpsTypeTraits<FModioUIBrushRef> : public TStructOpsTypeTraitsBase2<FModioUIBrushRef>
{
	enum
	{
		WithSerializer = true,
	};
};
*/