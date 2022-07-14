/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "IPropertyTypeCustomization.h"
#include "Templates/SharedPointer.h"

class FModioEditorHelpers
{
public:
	template<typename ImplementingClass>
	static TSharedRef<IPropertyTypeCustomization> MakePropertyCustomizationInstance()
	{
		return MakeShareable(new ImplementingClass());
	}

	template<typename ImplementingClass>
	static TSharedRef<IDetailCustomization> MakeDetailCustomizationInstance()
	{
		return MakeShareable(new ImplementingClass());
	}
	/// @brief Attempts to locate the property handle of a serialized proxy object so that it can be edited in place of
	/// the original property handle. The property handle should be located in a TMap<FName, ProxyType> on the outer
	/// UObject
	/// @tparam ProxyType Type of proxy object to find a handle for
	/// @param PropertyHandle The original handle to find a proxy for
	/// @param MetadataFieldName The metadata field on the UObject specifying the name of the map property to search
	/// @return Handle to existing proxy object, handle to new proxy object, or nothing if an error occurred
	template<typename ProxyType>
	static TSharedPtr<IPropertyHandle> FindSerializedProxyHandle(TSharedPtr<IPropertyHandle> PropertyHandle,
														  const FString& MetadataFieldName)
	{
		// Generate the path to the real FSlateColor being customized - this path will be used as the map key later,
		// so we can easily reapply the resolved color to the FSlateColor during PostLoad
		FString PropertyPath = PropertyHandle->GeneratePathToProperty();

		TArray<UObject*> OuterObjects;
		PropertyHandle->GetOuterObjects(OuterObjects);

		// Loop through each parent handle of the FSlateColor's property handle
		TSharedPtr<IPropertyHandle> OuterPropHandle = PropertyHandle->GetParentHandle();

		while (OuterPropHandle)
		{
			// UE_LOG(LogTemp, Log, TEXT("%s"), *OuterPropHandle->GeneratePathToProperty());
			// Sanity check, if we somehow get a property handle with a null property??
			if (!OuterPropHandle->GetProperty())
			{
				break;
			}
			// Check if this handle's associated property is a UClass
			UClass* Test = OuterPropHandle->GetProperty()->GetOwnerClass();
			// If it was a UClass and has the correct metadata in its hierarchy
			if (Test && Test->HasMetaDataHierarchical(FName(MetadataFieldName)))
			{
				// Resolve the name of the map we're going to serialize color data in
				FString SerializeFieldName;
				Test->GetStringMetaDataHierarchical(FName(MetadataFieldName), &SerializeFieldName);
				// The parent of the current property handle should be the property handle representing the class
				// object proper
				OuterPropHandle = OuterPropHandle->GetParentHandle();
				if (OuterPropHandle)
				{
					// Look on the class object for the property with the name stored in metadata
					TSharedPtr<IPropertyHandle> SerializationFieldPropHandle =
						OuterPropHandle->GetChildHandle(FName(SerializeFieldName));

					if (SerializationFieldPropHandle)
					{
						// UE_LOG(LogTemp, Log, TEXT("Serialized In %s"),
						//	   *SerializationFieldPropHandle->GeneratePathToProperty());
						FMapProperty* MapProp = CastField<FMapProperty>(SerializationFieldPropHandle->GetProperty());
						if (!MapProp)
						{
							break; // We could try to keep recursing up the tree, but safer for now to just fall
								   // back
						}
						TArray<const void*> PropertyRawData;
						SerializationFieldPropHandle->AccessRawData(PropertyRawData);
						if (PropertyRawData.Num())
						{
							// Search through the map to see if the value already exists
							int32 ValueIndex = INDEX_NONE;
							FScriptMapHelper Helper = FScriptMapHelper(MapProp, PropertyRawData[0]);
							for (int32 ChildIndex = 0; ChildIndex < Helper.Num(); ChildIndex++)
							{
								if (uint8* KeyPtr = Helper.GetKeyPtr(ChildIndex))
								{
									FName* KeyNamePtr = reinterpret_cast<FName*>(KeyPtr);
									if (*KeyNamePtr == FName(PropertyPath))
									{
										ValueIndex = ChildIndex;
									}
								}
							}
							if (ValueIndex == INDEX_NONE)
							{
								// The valye didn't exist, so we're going to add a new one to the map
								FName NewKey(PropertyPath);
								ProxyType NewValue {};
								TSharedPtr<IPropertyHandleMap> MapHandle = SerializationFieldPropHandle->AsMap();
								if (MapHandle)
								{
									MapHandle->AddItem();
									// We can't guarantee that the element will be added at the end of the map, so
									// iterate through it to find the handle of either a) the existing value or b)
									// the one we just added
									uint32 NumElements = 0;
									MapHandle->GetNumElements(NumElements);
									for (uint32 ChildIndex = 0; ChildIndex < NumElements; ChildIndex++)
									{
										TSharedPtr<IPropertyHandle> ElementHandle =
											SerializationFieldPropHandle->GetChildHandle(ChildIndex);
										if (ElementHandle.IsValid())
										{
											TSharedPtr<IPropertyHandle> ElementKeyHandle =
												ElementHandle->GetKeyHandle();
											if (ElementKeyHandle.IsValid())
											{
												FName KeyValue;
												if (ElementKeyHandle->GetValue(KeyValue) == FPropertyAccess::Success &&
													KeyValue == NAME_None)
												{
													ElementKeyHandle->SetValue(NewKey);
													TArray<void*> ElementRawData;
													ElementHandle->AccessRawData(ElementRawData);
													if (ElementRawData.Num())
													{
														ProxyType* ValuePtr =
															reinterpret_cast<ProxyType*>(ElementRawData[0]);
														*ValuePtr = NewValue;
													}

													ValueIndex = ChildIndex;
													MapHandle->GetNumElements(NumElements);
													break;
												}
											}
										}
									}
								}
							}
							// This should always be correct, because we either found an existing value, or we added one
							if (ValueIndex != INDEX_NONE)
							{
								uint32 NumChildren = 0;
								SerializationFieldPropHandle->GetNumChildren(NumChildren);
								// Get the handle of the specific entry in the map
								TSharedPtr<IPropertyHandle> ProxyHandle =
									SerializationFieldPropHandle->GetChildHandle(ValueIndex);
								return ProxyHandle;
							}
						}
					}
				}
			}
			OuterPropHandle = OuterPropHandle->GetParentHandle();
		}
		return nullptr;
	}
};