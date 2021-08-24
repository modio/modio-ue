#include "Types/ModioMetadata.h"
#include "ModioSDK.h"
#include "Internal/ModioConvert.h"
FModioMetadata::FModioMetadata(const Modio::Metadata& Metadata)
	: Key(ToUnreal(Metadata.Key)),
	  Value(ToUnreal(Metadata.Value))
{}
