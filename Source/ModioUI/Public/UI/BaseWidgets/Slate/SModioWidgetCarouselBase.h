#pragma once

#include "Blueprint/IUserListEntry.h"
#include "Core/Input/ModioUIDirection.h"
#include "Delegates/DelegateCombinations.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/SlateDelegates.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SFxWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SWidget.h"

UENUM()
enum class EModioWidgetCarouselSizeOverrideType
{
	AbsoluteDimensions,
	RelativeLayoutScale,
	NoOverride
};

class MODIOUI_API SModioWidgetCarouselEntry : public SFxWidget
{
protected:
	FVector InitialPosition;
	TSharedPtr<SWidget> MyContent;
	FVector2D LayoutScale = FVector2D(1, 1);
	float FocusScale = 1.2f;
	float CurrentFocusScale = 1.f;
	bool bFocusable = false;

public:
	void SetContent(TSharedRef<SWidget> InContent)
	{
		MyContent = InContent;
		ChildSlot[InContent];
	}
	virtual void SetInitialPosition(FVector Position)
	{
		InitialPosition = Position;
		SetVisualOffset(Position);
	};

	TSharedPtr<SWidget> GetContent()
	{
		return MyContent;
	}
	virtual bool SupportsKeyboardFocus() const override
	{
		return bFocusable;
	}
	// forward focus onto the content widget

	virtual void SetSelectionState(bool bNewState)
	{
		if (MyContent)
		{
			TSharedPtr<FModioUserWidgetMetaData> OwnerMetaData = MyContent->GetMetaData<FModioUserWidgetMetaData>();
			if (OwnerMetaData)
			{
				if (UModioUserWidgetBase* OwnerWidget = OwnerMetaData->GetAssociatedWidget())
				{
					IUserListEntry::UpdateItemSelection(*OwnerWidget, bNewState);
				}
			}
		}
	}

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		if (!MyContent)
		{
			return SFxWidget::OnFocusReceived(MyGeometry, InFocusEvent);
		}

		return FReply::Handled()
			.SetUserFocus(MyContent.ToSharedRef())
			.SetNavigation(MyContent.ToSharedRef(), ENavigationGenesis::User);
	}

	virtual FNavigationReply OnNavigation(const FGeometry& MyGeometry,
										  const FNavigationEvent& InNavigationEvent) override
	{
		return SFxWidget::OnNavigation(MyGeometry, InNavigationEvent);
	}

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (!HasAnyUserFocus().IsSet()) {}
	}

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		if (!HasAnyUserFocus().IsSet()) {}
	}

	virtual void SetFocusable(bool bNewFocusableState)
	{
		bFocusable = bNewFocusableState;
	}
	virtual void UpdateTransition(float Alpha, float WidgetSpacing, TOptional<bool> BeingFocused = {})
	{
		SetVisualOffset(InitialPosition + FVector(Alpha * WidgetSpacing, 0, 0));
		/*if (BeingFocused.IsSet())
		{
			CurrentFocusScale = 1.f + Alpha * (FocusScale - 1.f);
			if (BeingFocused.GetValue())
			{
				// Transition into focus
			}
			else
			{
				// Transition out of focus
			}
		}
		else
		{
			CurrentFocusScale = 1.f;
		}
		// No focus change, do nothing*/
	};

	void SetSizeOverride(FOptionalSize WidthOverride, FOptionalSize HeightOverride,
						 EModioWidgetCarouselSizeOverrideType OverrideType)
	{
		switch (OverrideType)
		{
			case EModioWidgetCarouselSizeOverrideType::NoOverride:

				ChildSlot[MyContent.ToSharedRef()];
				LayoutScale = FVector2D(1, 1);
				break;
			case EModioWidgetCarouselSizeOverrideType::AbsoluteDimensions:
				ChildSlot.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)[SNew(SBox)
											 .WidthOverride(WidthOverride)
											 .HeightOverride(HeightOverride)[MyContent.ToSharedRef()]];
				break;
			case EModioWidgetCarouselSizeOverrideType::RelativeLayoutScale:
				ChildSlot.HAlign(HAlign_Fill).VAlign(VAlign_Fill)[MyContent.ToSharedRef()];
				LayoutScale.X = WidthOverride.IsSet() ? WidthOverride.Get() : 1;
				LayoutScale.Y = HeightOverride.IsSet() ? HeightOverride.Get() : 1;
				break;
		}
	}

	FVector2D ComputeDesiredSize(float) const override
	{
		FVector2D DesiredUnderlyingSize = ChildSlot.GetWidget()->GetDesiredSize();
		return FVector2D(CurrentFocusScale * LayoutScale.X * DesiredUnderlyingSize.X,
						 CurrentFocusScale * LayoutScale.Y * DesiredUnderlyingSize.Y);
	}

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override
	{
		FReply Reply = FReply::Unhandled();
		if (FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept &&
			OnClicked.IsBound())
		{
			// Handle menu open with controller.
			Reply = OnClicked.Execute();
		}

		return Reply;
	}

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		FReply Reply = FReply::Unhandled();
		if (IsEnabled() && (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent()) &&
			OnClicked.IsBound())
		{
			Reply = OnClicked.Execute();
		}
		return Reply;
	}

	FOnClicked OnClicked;
};

/// @brief Templated base class for a widget which manages a series of sub-widgets, one per item, and rotates them in
/// and out
template<typename ItemType>
class MODIOUI_API SModioWidgetCarouselBase : public SCompoundWidget
{
public:
	using FOnGenerateWidget = typename TSlateDelegates<ItemType>::FOnGenerateWidget;

	DECLARE_DELEGATE_TwoParams(FOnCarouselEntryClicked, TSharedPtr<SModioWidgetCarouselEntry>, ItemType);

	SLATE_BEGIN_ARGS(SModioWidgetCarouselBase<ItemType>)
		: _ClippingBehaviour(EWidgetClipping::ClipToBounds),
		  _LeftCarouselWidget(nullptr),
		  _CenterCarouselWidget(nullptr),
		  _RightCarouselWidget(nullptr),
		  _IncomingWidget(nullptr),
		  _RelativeWidgetSpacing(1.25f)
	{}

	SLATE_EVENT(FOnGenerateWidget, OnGenerateWidget)

	SLATE_EVENT(FOnCarouselEntryClicked, OnLeftWidgetClicked)
	SLATE_EVENT(FOnCarouselEntryClicked, OnCenterWidgetClicked)
	SLATE_EVENT(FOnCarouselEntryClicked, OnRightWidgetClicked)

	SLATE_ARGUMENT(TArray<ItemType>, WidgetItemsSource)

	SLATE_ARGUMENT(EWidgetClipping, ClippingBehaviour)

	SLATE_ARGUMENT(TSharedPtr<SModioWidgetCarouselEntry>, LeftCarouselWidget)
	SLATE_ARGUMENT(TSharedPtr<SModioWidgetCarouselEntry>, CenterCarouselWidget)
	SLATE_ARGUMENT(TSharedPtr<SModioWidgetCarouselEntry>, RightCarouselWidget)
	SLATE_ARGUMENT(TSharedPtr<SModioWidgetCarouselEntry>, IncomingWidget)

	SLATE_ARGUMENT(float, RelativeWidgetSpacing)

	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs)
	{
		ItemsSource = InArgs._WidgetItemsSource;
		OnGenerateWidget = InArgs._OnGenerateWidget;

		OnLeftWidgetClicked = InArgs._OnLeftWidgetClicked;
		OnCenterWidgetClicked = InArgs._OnCenterWidgetClicked;
		OnRightWidgetClicked = InArgs._OnRightWidgetClicked;

		WidgetSpacing = InArgs._RelativeWidgetSpacing;

		LeftCarouselWidget = InArgs._LeftCarouselWidget ? InArgs._LeftCarouselWidget : SNew(SModioWidgetCarouselEntry);
		LeftCarouselWidget->SetClipping(InArgs._ClippingBehaviour);
		LeftCarouselWidget->SetVisibility(EVisibility::SelfHitTestInvisible);

		CenterCarouselWidget =
			InArgs._LeftCarouselWidget ? InArgs._CenterCarouselWidget : SNew(SModioWidgetCarouselEntry);
		CenterCarouselWidget->SetClipping(InArgs._ClippingBehaviour);
		CenterCarouselWidget->SetVisibility(EVisibility::SelfHitTestInvisible);

		RightCarouselWidget =
			InArgs._LeftCarouselWidget ? InArgs._RightCarouselWidget : SNew(SModioWidgetCarouselEntry);
		RightCarouselWidget->SetClipping(InArgs._ClippingBehaviour);
		RightCarouselWidget->SetVisibility(EVisibility::SelfHitTestInvisible);

		IncomingWidget = InArgs._LeftCarouselWidget ? InArgs._IncomingWidget : SNew(SModioWidgetCarouselEntry);
		IncomingWidget->SetClipping(InArgs._ClippingBehaviour);
		IncomingWidget->SetVisibility(EVisibility::SelfHitTestInvisible);

		// should this be called on the children instead?
		// SetClipping(InArgs._ClippingBehaviour);

		// clang-format off
		ChildSlot
		[
			SAssignNew(WidgetContainer, SHorizontalBox)
			+ SHorizontalBox::Slot()
				[
					SNew(SOverlay) 
					+ SOverlay::Slot()
						[
							LeftCarouselWidget.ToSharedRef()
						] 
					+ SOverlay::Slot()
						[
							CenterCarouselWidget.ToSharedRef()
						] 
					+ SOverlay::Slot()
						[
							RightCarouselWidget.ToSharedRef()
						]
					+ SOverlay::Slot()
						[
							IncomingWidget.ToSharedRef()
						]
				]
		];
		// clang-format on
		PopulateWidgetContainer();

		LeftCarouselWidget->SetInitialPosition(FVector(-WidgetSpacing, 0, 0));
		CenterCarouselWidget->SetInitialPosition(FVector(0, 0, 0));
		RightCarouselWidget->SetInitialPosition(FVector(WidgetSpacing, 0, 0));

		IncomingWidget->SetInitialPosition(FVector((-2 * WidgetSpacing), 0, 0));

		// UpdateWidgetVisibility();
		GenerateItemWidgets();

		UpdateClickHandlers();

		DefaultAnimCurve.AddCurve(0, 0.25f, ECurveEaseFunction::CubicInOut);
	}

	void UpdateSelection()
	{
		LeftCarouselWidget->SetSelectionState(false);
		RightCarouselWidget->SetSelectionState(false);
		CenterCarouselWidget->SetSelectionState(true);
		IncomingWidget->SetSelectionState(false);
	}

protected:
	float WidgetSpacing = 1.25f;

	FCurveSequence DefaultAnimCurve;

	bool IsInTransition()
	{
		return DefaultAnimCurve.IsPlaying();
	}

	int32 GetLeftWidgetIndex(int32 Index)
	{
		int32 LeftWidgetIndex = Index - 1;
		if (LeftWidgetIndex < 0)
		{
			LeftWidgetIndex = FMath::Max(0, ItemsSource.Num() - 1);
		}
		return LeftWidgetIndex;
	}

	int32 GetRightWidgetIndex(int32 Index)
	{
		int32 RightWidgetIndex = Index + 1;
		if (RightWidgetIndex >= ItemsSource.Num())
		{
			RightWidgetIndex = 0;
		}
		return RightWidgetIndex;
	}

	FReply LeftWidgetClickedHandler()
	{
		OnLeftWidgetClicked.Execute(LeftCarouselWidget, ItemsSource[GetLeftWidgetIndex(WidgetIndex)]);
		return FReply::Handled();
	}

	FReply CenterWidgetClickedHandler()
	{
		OnCenterWidgetClicked.Execute(CenterCarouselWidget, ItemsSource[WidgetIndex]);
		return FReply::Handled();
	}

	FReply RightWidgetClickedHandler()
	{
		OnRightWidgetClicked.Execute(RightCarouselWidget, ItemsSource[GetRightWidgetIndex(WidgetIndex)]);
		return FReply::Handled();
	}

	/// @brief swaps the pointers around based on the direction
	virtual void RotateWidgetReferences(EModioUIDirection Direction)
	{
		if (Direction == EModioUIDirection::MoveLeft)
		{
			// Old right widget is offscreen
			TSharedPtr<SModioWidgetCarouselEntry> TempWidget = RightCarouselWidget;
			RightCarouselWidget = CenterCarouselWidget;
			CenterCarouselWidget = LeftCarouselWidget;
			LeftCarouselWidget = IncomingWidget;
			// Keep old right widget as the spare widget for later
			IncomingWidget = TempWidget;
			// LeftCarouselWidget->SetSlide(CenterCarouselWidget->GetSlideValue() + SlideValueLeftLimit, true);
		}
		else
		{
			// Old left widget is offscreen
			TSharedPtr<SModioWidgetCarouselEntry> TempWidget = LeftCarouselWidget;
			LeftCarouselWidget = CenterCarouselWidget;
			CenterCarouselWidget = RightCarouselWidget;
			RightCarouselWidget = IncomingWidget;

			// Keep old left widget as the spare widget for later
			IncomingWidget = TempWidget;
			// RightCarouselWidget->SetSlide(CenterCarouselWidget->GetSlideValue() + SlideValueRightLimit, true);
		}
	}

	void UpdateClickHandlers()
	{
		LeftCarouselWidget->OnClicked = FOnClicked::CreateSP(this, &SModioWidgetCarouselBase::LeftWidgetClickedHandler);
		LeftCarouselWidget->SetFocusable(false);
		RightCarouselWidget->OnClicked =
			FOnClicked::CreateSP(this, &SModioWidgetCarouselBase::RightWidgetClickedHandler);
		RightCarouselWidget->SetFocusable(false);
		CenterCarouselWidget->OnClicked =
			FOnClicked::CreateSP(this, &SModioWidgetCarouselBase::CenterWidgetClickedHandler);
		CenterCarouselWidget->SetFocusable(true);
		IncomingWidget->OnClicked.Unbind();
		IncomingWidget->SetFocusable(false);
	}

	EActiveTimerReturnType RegisteredTick(double InCurrentTime, float InDeltaTime)
	{
		float TransitionPosition = DefaultAnimCurve.GetLerp();

		LeftCarouselWidget->UpdateTransition(TransitionPosition, WidgetSpacing);
		CenterCarouselWidget->UpdateTransition(TransitionPosition, WidgetSpacing, true);
		RightCarouselWidget->UpdateTransition(TransitionPosition, WidgetSpacing);
		IncomingWidget->UpdateTransition(TransitionPosition, WidgetSpacing);
		if (!IsInTransition())
		{
			UpdateClickHandlers();
			UpdateSelection();
		}
		return IsInTransition() ? EActiveTimerReturnType::Continue : EActiveTimerReturnType::Stop;
	}

	virtual void GenerateItemWidgets()
	{
		LeftCarouselWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[GetLeftWidgetIndex(WidgetIndex)]));
		CenterCarouselWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[WidgetIndex]));
		RightCarouselWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[GetRightWidgetIndex(WidgetIndex)]));
		// Incoming widget is just the next one again
		IncomingWidget->SetContent(
			OnGenerateWidget.Execute(ItemsSource[GetRightWidgetIndex(GetRightWidgetIndex(WidgetIndex))]));
		UpdateSelection();
	}

	virtual void PopulateWidgetContainer()
	{
		WidgetContainer->ClearChildren();

		// clang-format off
		WidgetContainer->AddSlot()
			[
				SNew(SOverlay) 
				+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						LeftCarouselWidget.ToSharedRef()
					] 
				+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						CenterCarouselWidget.ToSharedRef()
					] 
				+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						RightCarouselWidget.ToSharedRef()
					]
				+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						IncomingWidget.ToSharedRef()
					]
			];
		// clang-format on

		// In here, clear slots on the container, and add a new slot containing an overlay which in turn has slots
		// referencing the widgets through their pointers This allows us to easily shift the positions of the widgets
		// around, as the overlay is a panel that supports arbitrary widget locations
	}

	void UpdateWidgetPositions(EModioUIDirection MoveDirection)
	{
		if (MoveDirection == EModioUIDirection::MoveLeft)
		{
			LeftCarouselWidget->SetInitialPosition(FVector(-WidgetSpacing - WidgetSpacing, 0, 0));
			CenterCarouselWidget->SetInitialPosition(FVector(-WidgetSpacing, 0, 0));
			RightCarouselWidget->SetInitialPosition(FVector(-WidgetSpacing + WidgetSpacing, 0, 0));

			// We may wish to set this position *after* the animation has completed
			IncomingWidget->SetInitialPosition(FVector(-WidgetSpacing + (-2 * WidgetSpacing), 0, 0));
		}
		else
		{
			LeftCarouselWidget->SetInitialPosition(FVector(-WidgetSpacing, 0, 0));
			CenterCarouselWidget->SetInitialPosition(FVector(0, 0, 0));
			RightCarouselWidget->SetInitialPosition(FVector(WidgetSpacing, 0, 0));

			IncomingWidget->SetInitialPosition(FVector((2 * WidgetSpacing), 0, 0));
		}
		Invalidate(EInvalidateWidgetReason::LayoutAndVolatility);
	}

	virtual void MoveInDirection(EModioUIDirection Direction)
	{
		if (Direction == EModioUIDirection::MoveLeft)
		{
			WidgetIndex = GetLeftWidgetIndex(WidgetIndex);
		}
		else
		{
			WidgetIndex = GetRightWidgetIndex(WidgetIndex);
		}

		RotateWidgetReferences(Direction);

		UpdateClickHandlers();
		UpdateSelection();

		// UpdateWidgetVisibility();

		if (Direction == EModioUIDirection::MoveLeft)
		{
			LeftCarouselWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[GetLeftWidgetIndex(WidgetIndex)]));
			// IncomingWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[GetLeftWidgetIndex(WidgetIndex)]));
			// Then set the offset
		}
		else
		{
			RightCarouselWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[GetRightWidgetIndex(WidgetIndex)]));
			// IncomingWidget->SetContent(OnGenerateWidget.Execute(ItemsSource[GetRightWidgetIndex(WidgetIndex)]));
		}

		PopulateWidgetContainer();
		UpdateWidgetPositions(Direction);

		BeginTransition(Direction);
		// scroll all widgets in the specified direction
	}

	virtual void UpdateWidgetVisibility()
	{
		LeftCarouselWidget->SetVisibility(EVisibility::Visible);
		CenterCarouselWidget->SetVisibility(EVisibility::Visible);
		RightCarouselWidget->SetVisibility(EVisibility::Visible);
		IncomingWidget->SetVisibility(EVisibility::HitTestInvisible);
	}

	/// @brief Actually enables ticking and begins to tween the positions and transforms of the individual animations
	/// @param Direction The direction we are moving in
	virtual void BeginTransition(EModioUIDirection Direction)
	{
		// register to tick the widgets
		RegisterActiveTimer(0.0f,
							FWidgetActiveTimerDelegate::CreateSP(this, &SModioWidgetCarouselBase::RegisteredTick));
		if (Direction == EModioUIDirection::MoveLeft)
		{
			DefaultAnimCurve.Play(AsShared(), false, 0.f, false);
		}
		else
		{
			DefaultAnimCurve.PlayReverse(AsShared(), false, 0, false);
		}
		// Use an FCurveSequence by default for each of the transitions, and in registeredTick we can simply get the
		// alpha value of the curve and pass that along to our widgets widgets could have a Start and Update method, so
		// if you wanted to you could simply play your custom animation on Start and ignore update? set the goal
		// transforms of each of the widgets
	}

	TArray<ItemType> ItemsSource;

	FOnGenerateWidget OnGenerateWidget;

	TSharedPtr<SHorizontalBox> WidgetContainer;
	TSharedPtr<SModioWidgetCarouselEntry> LeftCarouselWidget;
	TSharedPtr<SModioWidgetCarouselEntry> CenterCarouselWidget;
	TSharedPtr<SModioWidgetCarouselEntry> RightCarouselWidget;
	TSharedPtr<SModioWidgetCarouselEntry> IncomingWidget;

	int32 WidgetIndex = 0;

public:
	virtual bool SupportsKeyboardFocus() const override
	{
		return true;
	}
	virtual void ScrollLeft()
	{
		MoveInDirection(EModioUIDirection::MoveLeft);
	};
	virtual void ScrollRight()
	{
		MoveInDirection(EModioUIDirection::MoveRight);
	};

	virtual FNavigationReply OnNavigation(const FGeometry& MyGeometry,
										  const FNavigationEvent& InNavigationEvent) override
	{
		// Possibly call OnNavigation first, then handle what would be 'escape' with the below code
		EUINavigation NavType = InNavigationEvent.GetNavigationType();

		if (NavType == EUINavigation::Left || NavType == EUINavigation::Previous)
		{
			ScrollLeft();
			FSlateApplication::Get().SetUserFocus(0, CenterCarouselWidget->GetContent(), EFocusCause::Navigation);
			return FNavigationReply::Explicit(CenterCarouselWidget->GetContent());
		}
		else if (NavType == EUINavigation::Right || NavType == EUINavigation::Next)
		{
			ScrollRight();
			FSlateApplication::Get().SetUserFocus(0, CenterCarouselWidget->GetContent(), EFocusCause::Navigation);
			return FNavigationReply::Explicit(CenterCarouselWidget->GetContent());
		}
		return SCompoundWidget::OnNavigation(MyGeometry, InNavigationEvent);
	}

	void SetItemSource(TArray<ItemType> InWidgetItemsSource)
	{
		ItemsSource = InWidgetItemsSource;
		GenerateItemWidgets();
	}

	void SetWidgetSpacing(float NewSpacing)
	{
		WidgetSpacing = NewSpacing;
		LeftCarouselWidget->SetInitialPosition(FVector(-WidgetSpacing, 0, 0));
		CenterCarouselWidget->SetInitialPosition(FVector(0, 0, 0));
		RightCarouselWidget->SetInitialPosition(FVector(WidgetSpacing, 0, 0));

		IncomingWidget->SetInitialPosition(FVector((-2 * WidgetSpacing), 0, 0));
	}

	void SetWidgetSizeOverride(FOptionalSize WidthOverride, FOptionalSize HeightOverride,
							   EModioWidgetCarouselSizeOverrideType OverrideType)
	{
		LeftCarouselWidget->SetSizeOverride(WidthOverride, HeightOverride, OverrideType);
		CenterCarouselWidget->SetSizeOverride(WidthOverride, HeightOverride, OverrideType);
		RightCarouselWidget->SetSizeOverride(WidthOverride, HeightOverride, OverrideType);
		IncomingWidget->SetSizeOverride(WidthOverride, HeightOverride, OverrideType);
	}

	int32 GetWidgetIndex() const
	{
		return WidgetIndex;
	}

	void SetWidgetIndex(int32 Index)
	{
		WidgetIndex = Index;
	}

	UPROPERTY(BlueprintAssignable)
	FOnCarouselEntryClicked OnLeftWidgetClicked;
	UPROPERTY(BlueprintAssignable)
	FOnCarouselEntryClicked OnCenterWidgetClicked;
	UPROPERTY(BlueprintAssignable)
	FOnCarouselEntryClicked OnRightWidgetClicked;
	// delegate for each element being clicked?

	// delegates for begin transition and end transition;
};