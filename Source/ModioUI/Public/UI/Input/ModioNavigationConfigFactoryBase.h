#pragma once

#include "Containers/Map.h"
#include "Core/Input/ModioInputKeys.h"
#include "Engine/DataAsset.h"
#include "Framework/Application/NavigationConfig.h"
#include "InputCoreTypes.h"
#include "Templates/SharedPointer.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UObject/Object.h"

#include "ModioNavigationConfigFactoryBase.generated.h"

class MODIOUI_API FModioUINavigationConfig : public FNavigationConfig, public IModioInputMappingAccessor
{
protected:
	TMap<FKey, EUINavigation> VirtualKeyToNavigationDirectionMap;
	TMap<FKey, EUINavigationAction> VirtualKeyToNavigationActionMap;

	virtual EUINavigation GetNavigationDirectionFromKey(const FKeyEvent& InKeyEvent) const
	{
		// Check if the physical key has a mapping to our virtual keys
		if (TOptional<FKey> VirtualKey = IModioInputMappingAccessor::GetCommandKeyForEvent(InKeyEvent))
		{
			// If the physical key has a mapping to a virtual key, check if that virtual key maps to a navigation
			// direction
			if (const EUINavigation* FoundDirection = VirtualKeyToNavigationDirectionMap.Find(VirtualKey.Get(FKey())))
			{
				return *FoundDirection;
			}
		}

		return EUINavigation::Invalid;
	}

	virtual EUINavigationAction GetNavigationActionFromKey(const FKeyEvent& InKeyEvent) const
	{
		// Check if the physical key has a mapping to our virtual keys
		if (TOptional<FKey> VirtualKey = IModioInputMappingAccessor::GetCommandKeyForEvent(InKeyEvent))
		{
			// If the physical key has a mapping to a virtual key, check if that virtual key maps to a navigation
			// action
			if (const EUINavigationAction* FoundAction = VirtualKeyToNavigationActionMap.Find(VirtualKey.Get(FKey())))
			{
				return *FoundAction;
			}
		}

		return EUINavigationAction::Invalid;
	}

	virtual EUINavigation GetNavigationDirectionFromAnalogInternal(const FAnalogInputEvent& InAnalogEvent)
	{
		/*if (bAnalogNavigation)
		{
			FUserNavigationState& UserState = UserNavigationState.FindOrAdd(InAnalogEvent.GetUserIndex());

			if (TOptional<FKey> VirtualKey = IModioInputMappingAccessor::GetCommandKeyForEvent(InKeyEvent))
			{
				if (VirtualKey.Get(FKey()) == FModioInputKeys::Left
				{
					if (InAnalogEvent.GetAnalogValue() < -AnalogNavigationHorizontalThreshold)
					{
						return EUINavigation::Left;
					}
					else if (InAnalogEvent.GetAnalogValue() > AnalogNavigationHorizontalThreshold)
					{
						return EUINavigation::Right;
					}
					else
					{
						UserState.AnalogNavigationState.Add(EUINavigation::Left, FAnalogNavigationState());
						UserState.AnalogNavigationState.Add(EUINavigation::Right, FAnalogNavigationState());
					}
				}
			}
			else if (InAnalogEvent.GetKey() == AnalogVerticalKey)
			{
				if (InAnalogEvent.GetAnalogValue() > AnalogNavigationVerticalThreshold)
				{
					return EUINavigation::Up;
				}
				else if (InAnalogEvent.GetAnalogValue() < -AnalogNavigationVerticalThreshold)
				{
					return EUINavigation::Down;
				}
				else
				{
					UserState.AnalogNavigationState.Add(EUINavigation::Up, FAnalogNavigationState());
					UserState.AnalogNavigationState.Add(EUINavigation::Down, FAnalogNavigationState());
				}
			}
		}
*/

		return EUINavigation::Invalid;
	}
	// Possibly store a focus stack someplace?
	virtual void OnNavigationChangedFocus(TSharedPtr<SWidget> OldWidget, TSharedPtr<SWidget> NewWidget,
										  FFocusEvent FocusEvent)
	{}

public:
	FModioUINavigationConfig() : FNavigationConfig()
	{
		// TODO: @modio-ue4-ui Do we want to map to conceptual next/previous as well?
		VirtualKeyToNavigationDirectionMap.Add(FModioInputKeys::Up, EUINavigation::Up);
		VirtualKeyToNavigationDirectionMap.Add(FModioInputKeys::Down, EUINavigation::Down);
		VirtualKeyToNavigationDirectionMap.Add(FModioInputKeys::Left, EUINavigation::Left);
		VirtualKeyToNavigationDirectionMap.Add(FModioInputKeys::Right, EUINavigation::Right);

		VirtualKeyToNavigationActionMap.Add(FModioInputKeys::Back, EUINavigationAction::Back);
		VirtualKeyToNavigationActionMap.Add(FModioInputKeys::Confirm, EUINavigationAction::Accept);
	}
};

UCLASS(abstract)
class MODIOUI_API UModioNavigationConfigFactoryBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual TSharedRef<FNavigationConfig> CreateNavigationConfig()
	{
		return MakeShared<FNavigationConfig>();
	}
};

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class MODIOUI_API UModioNavigationConfigFactory : public UModioNavigationConfigFactoryBase
{
	GENERATED_BODY()

protected:
	/** Should the Tab key perform next and previous style navigation. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere,  Category="ModioNavigationConfigFactory")
	bool bTabNavigation;

	/** Should we respect keys for navigation. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioNavigationConfigFactory")
	bool bKeyNavigation;

	/** Should we respect the analog stick for navigation. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioNavigationConfigFactory")
	bool bAnalogNavigation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioNavigationConfigFactory")
	float AnalogNavigationHorizontalThreshold;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioNavigationConfigFactory")
	float AnalogNavigationVerticalThreshold;

	virtual TSharedRef<FNavigationConfig> CreateNavigationConfig() override
	{
		// Need to subclass FNavigationConfig here so that we can alter the behaviour of the
		// analog navigation to check digital inputs as well
		TSharedRef<FModioUINavigationConfig> Config = MakeShared<FModioUINavigationConfig>();
		Config->bTabNavigation = bTabNavigation;
		Config->bKeyNavigation = bKeyNavigation;
		Config->bAnalogNavigation = bAnalogNavigation;
		Config->AnalogNavigationHorizontalThreshold = AnalogNavigationHorizontalThreshold;
		Config->AnalogNavigationVerticalThreshold = AnalogNavigationVerticalThreshold;

		return Config;
	}
};
