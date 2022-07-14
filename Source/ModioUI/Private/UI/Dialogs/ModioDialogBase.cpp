// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Dialogs/ModioDialogBase.h"
#include "Blueprint/WidgetTree.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"

bool UModioDialogBase::Initialize()
{
	bool bSuperResult = Super::Initialize();
	if (bSuperResult)
	{
		UWidget* Root = WidgetTree->ConstructWidget<UModioDialogBaseInternal>();
		WidgetTree->RootWidget = Root;
	}
	return bSuperResult;
}
