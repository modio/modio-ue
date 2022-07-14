#pragma once

#include "Containers/UnrealString.h"
#include "UObject/Interface.h"
#include "UObject/NameTypes.h"

#include "IModioUIStyleRefSerializer.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIStyleRefSerializer : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIStyleRefSerializer : public IInterface
{
	GENERATED_BODY()
public:
	virtual void NativeSerializeStyleReference(FString PathToProperty, FName StyleElementReference);
};