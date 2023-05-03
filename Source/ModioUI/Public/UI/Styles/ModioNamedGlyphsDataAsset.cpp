#include "ModioNamedGlyphsDataAsset.h"

UTexture2D* UModioNamedGlyphsDataAsset::GetNamedGlyph(FName GlyphName, bool bOverridePlatform)
{
	#if PLATFORM_XBOXONE || PLATFORM_PS4 || PLATFORM_PS5
	if (PNGGlyphs.Contains(GlyphName))
	{
		return PNGGlyphs.FindRef(GlyphName);
	}
	#else
	if (NamedGlyphs.Contains(GlyphName))
	{
		return bOverridePlatform ? PNGGlyphs.FindRef(GlyphName) : NamedGlyphs.FindRef(GlyphName);
	}
	#endif
	return nullptr;
}