#pragma once
#include "UObject/Interface.h"

#include "IModioUIUserStringListEntry.generated.h"


UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIUserStringListEntry : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIUserStringListEntry : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeSetListItemString(const FString& InString) {};
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetListItemString(const FString& InString);
    
	void SetListItemString_Implementation(const FString& InString)
	{
		NativeSetListItemString(InString);
	}
};