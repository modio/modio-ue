#pragma once
#include "UObject/Object.h"
#include "Widgets/Views/SListView.h"



template<typename ItemType = UObject*>
class MODIOUI_API SModioListView : public SListView<ItemType>
{
public:
	
	using FOnSelectionChanged = typename SListView<ItemType>::FOnSelectionChanged;
	
	FOnSelectionChanged& GetOnSelectionChanged()
	{
		return SListView<ItemType>::OnSelectionChanged;
	}
};
