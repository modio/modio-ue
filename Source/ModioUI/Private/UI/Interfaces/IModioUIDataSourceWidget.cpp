#include "UI/Interfaces/IModioUIDataSourceWidget.h"

void IModioUIDataSourceWidget::SetDataSource_Implementation(UObject* InDataSource)
{
	NativeSetDataSource(InDataSource);
}
