#pragma once
#include "UObject/Interface.h"

#include "IModioUICommand.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUICommand : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUICommand : public IInterface
{
	GENERATED_BODY()
protected:
	//virtual TSHaredPtr<FUICommandList> GetUICommands // implemented in interface
	//virtual TSharedPtr<FUICommandList> AddCommands; // implemented in derived classes

public:
	//virtual FReply HandleCommands(FKeyEvent Event) // virtual so can be overridden but default implementation in interface?

};