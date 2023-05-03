#include "ModioMaterialData.h"

UMaterialInterface* UModioMaterialData::GetGlyphMaterial(bool bOverridePlatform)
{
#if PLATFORM_XBOXONE || PLATFORM_PS4 || PLATFORM_PS5
	return PngGlyphMaterial;
	#else
	return bOverridePlatform ? PngGlyphMaterial : DefaultGlyphMaterial;
	#endif
}
