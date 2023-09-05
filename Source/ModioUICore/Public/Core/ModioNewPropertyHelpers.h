/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

// Copyright Epic Games, Inc. All Rights Reserved.
// This file contains an extended version of IteratePropertyPathRecursive because Epic do not currently support map
// properties.

#pragma once
#include "PropertyPathHelpers.h"
#include "Templates/EnableIf.h"
#include "Templates/UnrealTypeTraits.h"

namespace ModioNewPropertyHelpers
{
	using namespace PropertyPathHelpers;
	using namespace PropertyPathHelpersInternal;

	template<typename ContainerType>
	bool IteratePropertyPathRecursive(UStruct* InStruct, ContainerType* InContainer, int32 SegmentIndex,
									  const FCachedPropertyPath& InPropertyPath, FPropertyPathResolver& InResolver)
	{
		const FPropertyPathSegment& Segment = InPropertyPath.GetSegment(SegmentIndex);
		const int32 ArrayIndex = Segment.GetArrayIndex() == INDEX_NONE ? 0 : Segment.GetArrayIndex();

		// Reset cached address usage flag at the path root. This will be reset later in the recursion if conditions are
		// not met in the path.
		if (SegmentIndex == 0)
		{
#if DO_CHECK
			InPropertyPath.SetCachedContainer(InContainer);
#endif
			InPropertyPath.SetCanSafelyUsedCachedAddress(true);
		}

		// Obtain the property info from the given structure definition
		FFieldVariant Field = Segment.Resolve(InStruct);
		if (Field.IsValid())
		{
			const bool bFinalSegment = SegmentIndex == (InPropertyPath.GetNumSegments() - 1);

			if (FProperty* Property = Field.Get<FProperty>())
			{
				if (bFinalSegment)
				{
					return InResolver.Resolve(static_cast<ContainerType*>(InContainer), InPropertyPath);
				}
				else
				{
					// Check first to see if this is a simple object (eg. not an array of objects)
					if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
					{
						// Object boundary that can change, so we cant use the cached address safely
						InPropertyPath.SetCanSafelyUsedCachedAddress(false);

						// If it's an object we need to get the value of the property in the container first before we
						// can continue, if the object is null we safely stop processing the chain of properties.
						if (UObject* CurrentObject =
								ObjectProperty->GetPropertyValue_InContainer(InContainer, ArrayIndex))
						{
							return IteratePropertyPathRecursive(CurrentObject->GetClass(), CurrentObject,
																SegmentIndex + 1, InPropertyPath, InResolver);
						}
					}
					// Check to see if this is a simple weak object property (eg. not an array of weak objects).
					if (FWeakObjectProperty* WeakObjectProperty = CastField<FWeakObjectProperty>(Property))
					{
						// Object boundary that can change, so we cant use the cached address safely
						InPropertyPath.SetCanSafelyUsedCachedAddress(false);

						FWeakObjectPtr WeakObject =
							WeakObjectProperty->GetPropertyValue_InContainer(InContainer, ArrayIndex);

						// If it's an object we need to get the value of the property in the container first before we
						// can continue, if the object is null we safely stop processing the chain of properties.
						if (UObject* CurrentObject = WeakObject.Get())
						{
							return IteratePropertyPathRecursive(CurrentObject->GetClass(), CurrentObject,
																SegmentIndex + 1, InPropertyPath, InResolver);
						}
					}
					// Check to see if this is a simple soft object property (eg. not an array of soft objects).
					else if (FSoftObjectProperty* SoftObjectProperty = CastField<FSoftObjectProperty>(Property))
					{
						// Object boundary that can change, so we cant use the cached address safely
						InPropertyPath.SetCanSafelyUsedCachedAddress(false);

						FSoftObjectPtr SoftObject =
							SoftObjectProperty->GetPropertyValue_InContainer(InContainer, ArrayIndex);

						// If it's an object we need to get the value of the property in the container first before we
						// can continue, if the object is null we safely stop processing the chain of properties.
						if (UObject* CurrentObject = SoftObject.Get())
						{
							return IteratePropertyPathRecursive(CurrentObject->GetClass(), CurrentObject,
																SegmentIndex + 1, InPropertyPath, InResolver);
						}
					}
					// Check to see if this is a simple structure (eg. not an array of structures)
					else if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
					{
						// Recursively call back into this function with the structure property and container value
						return IteratePropertyPathRecursive<void>(
							StructProp->Struct, StructProp->ContainerPtrToValuePtr<void>(InContainer, ArrayIndex),
							SegmentIndex + 1, InPropertyPath, InResolver);
					}
					else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
					{
						// Dynamic array boundary that can change, so we cant use the cached address safely
						InPropertyPath.SetCanSafelyUsedCachedAddress(false);

						// It is an array, now check to see if this is an array of structures
						if (FStructProperty* ArrayOfStructsProp = CastField<FStructProperty>(ArrayProp->Inner))
						{
							FScriptArrayHelper_InContainer ArrayHelper(ArrayProp, InContainer);
							if (ArrayHelper.IsValidIndex(ArrayIndex))
							{
								// Recursively call back into this function with the array element and container value
								return IteratePropertyPathRecursive<void>(
									ArrayOfStructsProp->Struct, static_cast<void*>(ArrayHelper.GetRawPtr(ArrayIndex)),
									SegmentIndex + 1, InPropertyPath, InResolver);
							}
						}
						// if it's not an array of structs, maybe it's an array of classes
						// else if ( FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProp->Inner) )
						{
							// TODO Add support for arrays of objects.
						}
					}
					else if (FSetProperty* SetProperty = CastField<FSetProperty>(Property))
					{
						// TODO: we dont support set properties yet
					}
					else if (FMapProperty* MapProperty = CastField<FMapProperty>(Property))
					{
						// We know we aren't the last element so SegmentIndex+1 is safe to access, because this branch
						// is when bFinalSegment = false
						int32 MapElementIndex = InPropertyPath.GetSegment(SegmentIndex + 1).GetArrayIndex();
						if (MapElementIndex != INDEX_NONE)
						{
							FScriptMapHelper_InContainer MapHelper(MapProperty, InContainer);
							if (MapHelper.IsValidIndex(MapElementIndex))
							{
								if (FStructProperty* MapOfStructsProp =
										CastField<FStructProperty>(MapProperty->ValueProp))
								{
									if (InPropertyPath.GetNumSegments() >= SegmentIndex + 2)
									{
										return IteratePropertyPathRecursive<void>(
											MapOfStructsProp->Struct,
											static_cast<void*>(MapHelper.GetValuePtr(MapElementIndex)),
											SegmentIndex + 2, InPropertyPath, InResolver);
									}
								}
								else if (FObjectProperty* MapOfObjectsProp =
											 CastField<FObjectProperty>(MapProperty->ValueProp))
								{
									return IteratePropertyPathRecursive<void>(
										MapOfObjectsProp->PropertyClass,
										static_cast<void*>(MapHelper.GetValuePtr(MapElementIndex)), SegmentIndex + 2,
										InPropertyPath, InResolver);
								}
							}
						}
					}
				}
			}
			else
			{
				// If it's the final segment, use the resolver to get the value.
				if (bFinalSegment)
				{
					return InResolver.Resolve(static_cast<ContainerType*>(InContainer), InPropertyPath);
				}
				else
				{
					// If it's not the final segment, but still a function, we're going to treat it as an Object*
					// getter. in the hopes that it leads to another object that we can resolve the next segment on.
					// These getter functions must be very simple.

					UObject* CurrentObject = nullptr;
					FProperty* GetterProperty = nullptr;
					FInternalGetterResolver<UObject*> GetterResolver(CurrentObject, GetterProperty);

					FCachedPropertyPath TempPath(Segment);
					if (GetterResolver.Resolve(InContainer, TempPath))
					{
						if (CurrentObject)
						{
							InPropertyPath.SetCanSafelyUsedCachedAddress(false);

							return IteratePropertyPathRecursive(CurrentObject->GetClass(), CurrentObject,
																SegmentIndex + 1, InPropertyPath, InResolver);
						}
					}
				}
			}
		}

		return false;
	}

	FORCEINLINE bool ResolvePropertyPath(UObject* InContainer, const FString& InPropertyPath,
										 FPropertyPathResolver& InResolver)
	{
		if (InContainer)
		{
			FCachedPropertyPath InternalPropertyPath(InPropertyPath);
			return ModioNewPropertyHelpers::IteratePropertyPathRecursive<UObject>(InContainer->GetClass(), InContainer,
																				  0, InternalPropertyPath, InResolver);
		}

		return false;
	}
	FORCEINLINE bool ResolvePropertyPath(UObject* InContainer, const FCachedPropertyPath& InPropertyPath,
										 FPropertyPathResolver& InResolver)
	{
		if (InContainer)
		{
			return ModioNewPropertyHelpers::IteratePropertyPathRecursive<UObject>(InContainer->GetClass(), InContainer,
																				  0, InPropertyPath, InResolver);
		}

		return false;
	}

	template<typename StructContainerType>
	FORCEINLINE bool ResolvePropertyPath(StructContainerType* InContainer, const FCachedPropertyPath& InPropertyPath,
										 FPropertyPathResolver& InResolver)
	{
		if (InContainer)
		{
			return ModioNewPropertyHelpers::IteratePropertyPathRecursive<StructContainerType>(
				StructContainerType::StaticStruct(), InContainer, 0, InPropertyPath, InResolver);
		}
	}

	template<typename T>
	bool SetPropertyValue(UObject* InContainer, const FCachedPropertyPath& InPropertyPath, const T& InValue)
	{
		check(InContainer);

		if (InPropertyPath.IsFullyResolved())
		{
			return SetPropertyValueFast<T>(InContainer, InPropertyPath, InValue);
		}
		else
		{
			PropertyPathHelpersInternal::FInternalSetterResolver<T> Resolver(InValue);
			return ModioNewPropertyHelpers::ResolvePropertyPath(InContainer, InPropertyPath, Resolver);
		}
	}

	template<typename StructContainerType, typename T>
	bool SetPropertyValueForStruct(StructContainerType* InContainer, const FCachedPropertyPath& InPropertyPath,
								   const T& InValue)
	{
		check(InContainer);
		PropertyPathHelpersInternal::FInternalSetterResolver<T> Resolver(InValue);
		return ModioNewPropertyHelpers::ResolvePropertyPath<StructContainerType>(InContainer, InPropertyPath, Resolver);
	}

} // namespace ModioNewPropertyHelpers