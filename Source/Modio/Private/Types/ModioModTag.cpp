#include "Types/ModioModTag.h"
#include "Internal/ModioConvert.h"
FModioModTag::FModioModTag(const Modio::ModTag& ModTag) : Tag(ToUnreal(ModTag.Tag)) {}
