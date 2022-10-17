/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/Slate/SModioCustomComboBox.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioCheckBoxStyle.h"
#include "UI/Styles/ModioCodeInputStyle.h"
#include "UI/Styles/ModioDialogStyle.h"
#include "UI/Styles/ModioDownloadQueueEntryStyle.h"
#include "UI/Styles/ModioDrawerWidgetStyle.h"
#include "UI/Styles/ModioEditableTextBoxStyle.h"
#include "UI/Styles/ModioMultiLineEditableTextBoxStyle.h"
#include "UI/Styles/ModioNotificationStyle.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioSubscriptionBadgeStyle.h"
#include "UI/Styles/ModioTagWidgetStyle.h"
#include "UI/Styles/ModioTextBlockStyleOverride.h"
#include "UI/Styles/ModioModTileStyle.h"
#include "UI/Styles/ModioPopupMenuStyle.h"

const FName FModioButtonStyle::TypeName = FName("ModioButtonStyle");
const FName FModioRichTextStyle::TypeName = FName("ModioRichTextStyle");
const FName FModioDrawerWidgetStyle::TypeName = FName("ModioDrawerWidgetStyle");
const FName FModioDialogStyle::TypeName = FName("ModioDialogStyle");
const FName FModioMultiLineEditableTextBoxStyle::TypeName = FName("ModioMultiLineEditableTextBoxStyle");
const FName FModioEditableTextBoxStyle::TypeName = FName("ModioEditableTextBoxStyle");
const FName FModioCodeInputStyle::TypeName = FName("ModioCodeInputStyle");
const FName FModioTextBlockStyleOverride::TypeName = FName("ModioTextBlockStyleOverride");
const FName FModioDownloadQueueEntryStyle::TypeName = FName("ModioDownloadQueueEntryStyle");
const FName FModioSubscriptionBadgeStyle::TypeName = FName("ModioSubscriptionBadgeStyle");
const FName FModioTagWidgetStyle::TypeName = FName("ModioTagWidgetStyle");
const FName FModioCheckBoxStyle::TypeName = FName("ModioCheckBoxStyle");
const FName FModioNotificationStyle::TypeName = FName("ModioNotificationStyle");
const FName FModioModTileStyle::TypeName = FName("ModioModTileStyle");
const FName FModioPopupMenuStyle::TypeName = FName("ModioPopupMenuStyle");