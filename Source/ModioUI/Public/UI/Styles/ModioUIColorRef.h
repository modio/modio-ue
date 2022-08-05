#pragma once
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "UObject/NameTypes.h"

#include "ModioUIColorRef.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIColorRef
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Color")
	FName ColorName;

	friend class FModioSlateColorDetailsCustomization;

public:
	/// @brief Initialize an empty color reference
	FModioUIColorRef() : ColorName("White") {};

	/// @brief Initialize a reference to the specified named color
	/// @param ColorName The name of the color to reference
	FModioUIColorRef(FName ColorName) : ColorName(ColorName) {};

	operator FSlateColor() const;

	/// @brief Updates the reference to point to a new named color
	/// @param NewColorName The name of the color to reference
	void SetLinkedColor(FName NewColorName);

	/// @brief Resolves the named reference to a Slate Color object sharing the underlying color amongst all other named
	/// references This means that updates to the style the reference was resolved against will propagate to anything
	/// that uses the FSlateColor to retrieve the color value
	/// @param OverrideStyleSet By default the reference will be resolved against the default style set, this provides a
	/// different styleset to reference. Not intended for normal use, currently used only to break a circular dependency
	/// when setting a new UModioUIStyleSet.
	/// @return FSlateColor object with shared reference to the named color, or a default white color if the name was
	/// invalid
	FSlateColor ResolveReference(UModioUIStyleSet* OverrideStyleSet = nullptr) const;
	/// @brief Returns a shared slate brush instance which uses the underlying named color as it's tint value
	/// @return shared brush containing the color
	const FSlateBrush* GetBrush() const;

	/// @brief Resolves the named color and then applies it as the tint color of the specified brush
	/// @param Target The brush to update with the resolved color value
	void ApplyToBrush(FSlateBrush* Target) const;

	bool Serialize(FArchive& Ar);
};

template<>
struct TStructOpsTypeTraits<FModioUIColorRef> : public TStructOpsTypeTraitsBase2<FModioUIColorRef>
{
	enum
	{
		WithSerializer = true,
	};
};