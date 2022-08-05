#pragma once

#include "Styling/SlateBrush.h"
#include "UObject/Interface.h"

#include "IModioUIImageDisplay.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIImageDisplay : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIImageDisplay : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeDisplayImage(const FSlateBrush& Image);
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ModioUIImageDisplay")
	void DisplayImage(const FSlateBrush& Image);

	void DisplayImage_Implementation(const FSlateBrush& Image)
	{
		NativeDisplayImage(Image);
	}
};