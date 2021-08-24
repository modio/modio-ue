#pragma once

// clang-format off
#include "ModioImageState.generated.h"
// clang-format on

UENUM(BlueprintType)
enum class EModioImageState : uint8
{
	OnDisc,
	LoadingIntoMemory,
	InMemory,
	Corrupted
};