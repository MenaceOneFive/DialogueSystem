#include "Graph/Slate/PropertyEditor/DialogueSelectionNodeDetail.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE "FDialogueSelectionNodeDetail"

TSharedRef<IDetailCustomization> FDialogueSelectionNodeDetail::MakeInstance()
{
    return MakeShareable(new FDialogueSelectionNodeDetail);
}

void FDialogueSelectionNodeDetail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    FDialogueNodeDetail::CustomizeDetails(DetailBuilder);
}

void FDialogueSelectionNodeDetail::CreateMulticastEventCustomization(IDetailLayoutBuilder& DetailBuilder)
{
    // UBlueprint* BlueprintObj = Blueprint.Get();
    // if ( BlueprintObj == nullptr )
    // {
    //     return;
    // }
    //
    // const FString AddString  = FString(TEXT("Add "));
    // const FString ViewString = FString(TEXT("View "));
    //
    // const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
    //
    // if ( !K2Schema->CanUserKismetAccessVariable(DelegateProperty, PropertyClass, UEdGraphSchema_K2::MustBeDelegate) )
    // {
    //     return;
    // }
    //
    // FText PropertyTooltip = DelegateProperty->GetToolTipText();
    // if ( PropertyTooltip.IsEmpty() )
    // {
    //     PropertyTooltip = FText::FromString(DelegateProperty->GetName());
    // }
    //
    // static FText EventCategoryText        = LOCTEXT("Events", "Events");
    // IDetailCategoryBuilder& EventCategory = DetailBuilder.EditCategory(TEXT("Events"), EventCategoryText, ECategoryPriority::Uncommon);
    // FObjectProperty* ComponentProperty    = FindFProperty<FObjectProperty>(BlueprintObj->SkeletonGeneratedClass, ThisComponentName);
    // if ( ComponentProperty )
    // {
    //     FName PropertyName = ComponentProperty->GetFName();
    //     FName EventName    = DelegateProperty->GetFName();
    //     FText EventText    = DelegateProperty->GetDisplayNameText();
    //
    //     EventCategory.AddCustomRow(EventText)
    //                  .WholeRowContent()
    //     [
    //         SNew(SHorizontalBox)
    //         .ToolTipText(DelegateProperty->GetToolTipText())
    //         + SHorizontalBox::Slot()
    //         .AutoWidth()
    //         .VAlign(VAlign_Center)
    //         .Padding(0.0f, 0.0f, 5.0f, 0.0f)
    //         [
    //             SNew(SImage)
    //             .Image(FAppStyle::Get().GetBrush("GraphEditor.Event_16x"))
    //         ]
    //
    //         + SHorizontalBox::Slot()
    //         .VAlign(VAlign_Center)
    //         .HAlign(HAlign_Left)
    //         [
    //             SNew(STextBlock)
    //             .Font(IDetailLayoutBuilder::GetDetailFont())
    //             .Text(EventText)
    //         ]
    //
    //         + SHorizontalBox::Slot()
    //         .HAlign(HAlign_Left)
    //         .VAlign(VAlign_Center)
    //         .Padding(0.0f)
    //         [
    //             SNew(SButton)
    //             .ContentPadding(FMargin(3.0f, 2.0f))
    //             .OnClicked(this, &FBlueprintWidgetCustomization::HandleAddOrViewEventForVariable, EventName, PropertyName, MakeWeakObjectPtr(PropertyClass))
    //             [
    //                 SNew(SWidgetSwitcher)
    //                 .WidgetIndex(this, &FBlueprintWidgetCustomization::HandleAddOrViewIndexForButton, EventName, PropertyName)
    //                 + SWidgetSwitcher::Slot()
    //                 [
    //                     SNew(SImage)
    //                     .ColorAndOpacity(FSlateColor::UseForeground())
    //                     .Image(FAppStyle::Get().GetBrush("Icons.SelectInViewport"))
    //                 ]
    //                 + SWidgetSwitcher::Slot()
    //                 [
    //                     SNew(SImage)
    //                     .ColorAndOpacity(FSlateColor::UseForeground())
    //                     .Image(FAppStyle::Get().GetBrush("Icons.Plus"))
    //                 ]
    //             ]
    //         ]
    //     ];
    // }
    // else if ( !bCreateMulticastEventCustomizationErrorAdded )
    // {
    //     bCreateMulticastEventCustomizationErrorAdded = true;
    //     EventCategory.AddCustomRow(FText::GetEmpty())
    //                  .WholeRowContent()
    //     [
    //         SNew(STextBlock)
    //         .Font(IDetailLayoutBuilder::GetDetailFont())
    //         .Text(LOCTEXT("EventAvailableButNotVariable", "To see available events, enable the Is Variable setting for this widget."))
    //     ];
    // }
}

#undef LOCTEXT_NAMESPACE
