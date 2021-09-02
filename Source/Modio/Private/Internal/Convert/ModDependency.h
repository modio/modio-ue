#pragma once
#include "Internal/ModioConvert.h"

FORCEINLINE FModioModDependency ToUnreal(const Modio::ModDependency& In)
{
	FModioModDependency Out;
	Out.ModID = ToUnreal(In.ModID);
	Out.ModName = ToUnreal(In.ModName);
	return Out;
}