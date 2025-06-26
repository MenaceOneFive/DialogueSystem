// ReSharper disable CppPassValueParameterByConstReference
#include "Graph/Slate/DialogueNodeDetail.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"
#include "Engine/Blueprint.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

#define LOCTEXT_NAMESPACE "FDialogueNodeDetail"

TSharedRef<IDetailCustomization> FDialogueNodeDetail::MakeInstance()
{
    return MakeShared<FDialogueNodeDetail>();
}

FReply FDialogueNodeDetail::OnOpenFunctionGraph(const FName& FunctionName) const
{
    if ( FunctionName.IsEqual(NAME_None) )
    {
        FNotificationInfo Info(LOCTEXT("FunctionReferenceWasNotSet", "Function Reference was not set!"));
        Info.ExpireDuration = 2.0f;
        FSlateNotificationManager::Get().AddNotification(Info);
        return FReply::Handled();
    }

    const auto Asset       = EdGraphNode->GetTypedOuter<UDialogueGraph>();
    const auto Blueprint   = Asset->GetBlueprintInstance();
    const auto GraphItSelf = Blueprint->FunctionGraphs.FindByPredicate([FunctionName](const TObjectPtr<UEdGraph>& Graph)
    {
        return Graph->GetName() == FunctionName;
    });

    if ( !GraphItSelf )
    {
        FNotificationInfo Info(FText::Format(LOCTEXT("FunctionNotExists", "Failed to open function {0} doesn't exists"), FText::FromName(FunctionName)));
        Info.ExpireDuration = 2.0f;
        FSlateNotificationManager::Get().AddNotification(Info);
        return FReply::Handled();
    }

    FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphItSelf->Get());

    return FReply::Handled();
}

FReply FDialogueNodeDetail::OnOpenCanSelectFunction() const
{
    const auto CanSelectThisNode = Cast<ICanSelectThisNode>(EdGraphNode);
    check(CanSelectThisNode)

    const auto FunctionName = CanSelectThisNode->GetCanSelectThisNodeFunctionName();
    return OnOpenFunctionGraph(FunctionName);
}

FReply FDialogueNodeDetail::OnOpenWhenSelectFunction() const
{
    const auto WhenSelectThisNode = Cast<IWhenSelectThisNode>(EdGraphNode);
    check(WhenSelectThisNode)

    const auto FunctionName = WhenSelectThisNode->GetWhenSelectThisNodeFunctionName();

    return OnOpenFunctionGraph(FunctionName);
}

void FDialogueNodeDetail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> OutObjects;
    DetailBuilder.GetObjectsBeingCustomized(OutObjects);
    if ( OutObjects.IsEmpty() )
    {
        return;
    }
    const auto ObjectBeingEdited = OutObjects[0];
    if ( const auto Node = ObjectBeingEdited.Pin() )
    {
        IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("NodeSelectionEvents", FText::FromString(TEXT("Node Selection Events")));

        if ( const auto GraphNode = Cast<UDialogueEdGraphNode>(Node.Get()) )
        {
            auto& Group = Category.AddGroup(FName("NodeId"), FText::FromString(TEXT("NodeId")), false, true);
            Group.AddWidgetRow()[
                SNew(STextBlock)
                .Text(FText::FromString(GraphNode->NodeGuid.ToString()))
            ];
        }
        if ( const auto GraphNode = Cast<UDialogueEdGraphNode>(Node.Get());
            GraphNode->Implements<UCanSelectThisNode>() ||
            GraphNode->Implements<UWhenSelectThisNode>() )
        {
            EdGraphNode = GraphNode; // 멤버 변수에 그래프 노드 저장

            if ( GraphNode->Implements<UCanSelectThisNode>() )
            {
                auto& Group = Category.AddGroup(FName("CanSelectThisNode"), FText::FromString(TEXT("Can select this node")), false, true);
                Group.HeaderRow()
                [
                    SNew(SButton)
                    .Text(FText::FromString(TEXT("편집")))
                    .OnClicked(this, &FDialogueNodeDetail::OnOpenCanSelectFunction)
                ];
                Group.AddWidgetRow()
                [
                    SAssignNew(CanSelectComboButton, SComboButton)
                    .OnGetMenuContent(this, &FDialogueNodeDetail::OnGetMenuContent_CanSelect)
                    .ButtonContent()
                    [
                        SNew(STextBlock)
                        .Text(this, &FDialogueNodeDetail::GetCurrentFunctionName_CanSelect)
                    ]
                ];
            }

            if ( GraphNode->Implements<UWhenSelectThisNode>() )
            {
                auto& Group = Category.AddGroup(FName("WhenSelectThisNode"), FText::FromString(TEXT("When select this node")), false, true);
                Group.HeaderRow()
                [
                    SNew(SButton)
                    .Text(FText::FromString(TEXT("편집")))
                    .OnClicked(this, &FDialogueNodeDetail::OnOpenWhenSelectFunction)
                ];
                Group.AddWidgetRow()
                [
                    SAssignNew(WhenSelectComboButton, SComboButton)
                    .OnGetMenuContent(this, &FDialogueNodeDetail::OnGetMenuContent_WhenSelect)
                    .ButtonContent()
                    [
                        SNew(STextBlock)
                        .Text(this, &FDialogueNodeDetail::GetCurrentFunctionName_WhenSelect)
                    ]
                ];
            }
        }
    }
}

TSharedRef<SWidget> FDialogueNodeDetail::OnGetMenuContent_CanSelect()
{
    CanSelectFunctionNames.Empty();
    CanSelectFunctionNames.Add(MakeShared<FName>(NAME_None));
    CanSelectFunctionNames.Add(MakeShared<FName>(CreateNewFunction));

    if ( const UClass* GeneratedClass = GetBlueprintGeneratedClass() )
    {
        for ( TFieldIterator<UFunction> FuncIt(GeneratedClass, EFieldIteratorFlags::IncludeSuper); FuncIt; ++FuncIt )
        {
            if ( const UFunction* Function = *FuncIt; Function && Function->HasAnyFunctionFlags(FUNC_BlueprintCallable) )
            {
                int NumberOfDialogueNodeType = 0;
                bool HasBooleanReturnValue   = false;

                for ( TFieldIterator<FProperty> It(Function); It; ++It )
                {
                    if ( const auto ObjectProp = CastField<FObjectProperty>(*It); ObjectProp && ObjectProp->HasAnyPropertyFlags(CPF_Parm) && ObjectProp->PropertyClass->IsChildOf<UDialogueGraphNode>() )
                    {
                        NumberOfDialogueNodeType++;
                        continue;
                    }
                    if ( const auto BoolProp = CastField<FBoolProperty>(*It); BoolProp && BoolProp->HasAnyPropertyFlags(CPF_Parm | CPF_OutParm | CPF_ReturnParm) )
                    {
                        HasBooleanReturnValue = true;
                    }
                }

                if ( HasBooleanReturnValue && NumberOfDialogueNodeType == 2 )
                {
                    CanSelectFunctionNames.Add(MakeShared<FName>(Function->GetFName()));
                }
            }
        }
    }

    return SNew(SListView<TSharedPtr<FName>>)
        .ListItemsSource(&CanSelectFunctionNames)
        .OnGenerateRow(this, &FDialogueNodeDetail::OnGenerateRowForNameListView)
        .OnSelectionChanged(this, &FDialogueNodeDetail::OnSelectionChanged_CanSelect);
}

FString FDialogueNodeDetail::MakeFunctionName(EFunctionType FunctionType,
                                              const UDialogueEdGraphNode* Node) const
{
    const auto NodeTypeName = Node->GetClass()->GetName();
    auto GUID               = Node->NodeGuid;
    auto GUIDString         = FString::Format(TEXT("{0}_{1}"), {GUID.C, GUID.D});

    switch ( FunctionType )
    {
    case EFunctionType::CanSelectThisNode:
        return FString::Format(TEXT("CanSelectThisNode_{0}"), {GUIDString});
    case EFunctionType::WhenSelectThisNode:
        return FString::Format(TEXT("WhenSelectThisNode_{0}"), {GUIDString});
    default:
        return FString::Format(TEXT("UnknownNodeEvent_{0}"), {GUIDString});
    }
}

TSharedRef<SWidget> FDialogueNodeDetail::OnGetMenuContent_WhenSelect()
{
    WhenSelectFunctionNames.Empty();
    WhenSelectFunctionNames.Add(MakeShared<FName>(NAME_None));
    WhenSelectFunctionNames.Add(MakeShared<FName>(CreateNewFunction));

    if ( const UClass* GeneratedClass = GetBlueprintGeneratedClass() )
    {
        for ( TFieldIterator<UFunction> FuncIt(GeneratedClass, EFieldIteratorFlags::IncludeSuper); FuncIt; ++FuncIt )
        {
            if ( const UFunction* Function = *FuncIt; Function && Function->HasAnyFunctionFlags(FUNC_BlueprintCallable) )
            {
                int NumberOfDialogueNodeType = 0;
                bool HasBooleanReturnValue   = false;


                for ( TFieldIterator<FProperty> It(Function); It; ++It )
                {
                    if ( const auto ObjectProp = CastField<FObjectProperty>(*It); ObjectProp && ObjectProp->HasAnyPropertyFlags(CPF_Parm) && ObjectProp->PropertyClass->IsChildOf<UDialogueGraphNode>() )
                    {
                        NumberOfDialogueNodeType++;
                        continue;
                    }
                    if ( const auto BoolProp = CastField<FBoolProperty>(*It); BoolProp && BoolProp->HasAnyPropertyFlags(CPF_Parm | CPF_OutParm | CPF_ReturnParm) )
                    {
                        HasBooleanReturnValue = true;
                    }
                }

                if ( !HasBooleanReturnValue && NumberOfDialogueNodeType == 2 )
                {
                    WhenSelectFunctionNames.Add(MakeShared<FName>(Function->GetFName()));
                }
            }
        }
    }

    return SNew(SListView<TSharedPtr<FName>>)
        .ListItemsSource(&WhenSelectFunctionNames)
        .OnGenerateRow(this, &FDialogueNodeDetail::OnGenerateRowForNameListView)
        .OnSelectionChanged(this, &FDialogueNodeDetail::OnSelectionChanged_WhenSelect);
}

TSharedRef<ITableRow> FDialogueNodeDetail::OnGenerateRowForNameListView(TSharedPtr<FName> InItem,
                                                                        const TSharedRef<STableViewBase>& OwnerTable) const
{
    return SNew(STableRow<TSharedPtr<FName>>, OwnerTable)
        [
            SNew(STextBlock).Text(FText::FromName(*InItem.Get()))
        ];
}

void FDialogueNodeDetail::OnSelectionChanged_CanSelect(TSharedPtr<FName> NewSelection,
                                                       ESelectInfo::Type SelectInfo) const
{
    if ( NewSelection.IsValid() && EdGraphNode.IsValid() )
    {
        if ( const auto CanSelectThisNode = Cast<ICanSelectThisNode>(EdGraphNode.Get()) )
        {
            EdGraphNode->Modify();
            if ( NewSelection->IsEqual(CreateNewFunction) )
            {
                FScopedTransaction ScopedTransaction{FText::FromString("Creating Function for CanSelectThisItem")};
                check(EdGraphNode.IsValid())
                const auto Asset = EdGraphNode->GetTypedOuter<UDialogueGraph>();
                check(Asset)
                const auto Blueprint = Asset->GetBlueprintInstance();
                Blueprint->Modify();
                Asset->Modify();

                const FString NodeName = MakeFunctionName(EFunctionType::CanSelectThisNode, EdGraphNode.Get());
                const auto NewFunction = CreateNewFunction_CanSelect(FName(NodeName));

                EdGraphNode->Modify();
                CanSelectThisNode->SetCanSelectThisNodeFunctionName(*NodeName);

                FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NewFunction);
            }
            else
            {
                FScopedTransaction ScopedTransaction{FText::FromString("Setting Function for CanSelectThisItem")};
                EdGraphNode->Modify();
                CanSelectThisNode->SetCanSelectThisNodeFunctionName(*NewSelection.Get());
            }
        }
    }

    if ( CanSelectComboButton.IsValid() )
    {
        CanSelectComboButton->SetIsOpen(false);
    }
}

void FDialogueNodeDetail::OnSelectionChanged_WhenSelect(TSharedPtr<FName> NewSelection,
                                                        ESelectInfo::Type SelectInfo) const
{
    if ( NewSelection.IsValid() && EdGraphNode.IsValid() )
    {
        if ( const auto WhenSelectThisNode = Cast<IWhenSelectThisNode>(EdGraphNode.Get()) )
        {
            EdGraphNode->Modify();
            if ( NewSelection->IsEqual(CreateNewFunction) )
            {
                FScopedTransaction ScopedTransaction{FText::FromString("Creating Function for WhenSelectThisItem")};
                check(EdGraphNode.IsValid())
                const auto Asset = EdGraphNode->GetTypedOuter<UDialogueGraph>();

                check(Asset)
                const auto Blueprint = Asset->GetBlueprintInstance();
                Blueprint->Modify();
                Asset->Modify();
                EdGraphNode->Modify();
                const FString NodeName = MakeFunctionName(EFunctionType::WhenSelectThisNode, EdGraphNode.Get());
                const auto NewFunction = CreateNewFunction_WhenSelect(FName(NodeName));

                WhenSelectThisNode->SetWhenSelectThisNodeFunctionName(*NodeName);

                FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NewFunction);
            }
            else
            {
                FScopedTransaction ScopedTransaction{FText::FromString("Setting Function for WhenSelectThisItem")};
                EdGraphNode->Modify();
                WhenSelectThisNode->SetWhenSelectThisNodeFunctionName(*NewSelection.Get());
            }
        }
    }

    if ( WhenSelectComboButton.IsValid() )
    {
        WhenSelectComboButton->SetIsOpen(false);
    }
}

FText FDialogueNodeDetail::GetCurrentFunctionName_CanSelect() const
{
    if ( EdGraphNode.IsValid() )
    {
        if ( const auto CanSelectThisNode = Cast<ICanSelectThisNode>(EdGraphNode.Get()) )
        {
            return FText::FromName(CanSelectThisNode->GetCanSelectThisNodeFunctionName());
        }
    }
    return FText::FromString(TEXT("Select Function..."));
}

FText FDialogueNodeDetail::GetCurrentFunctionName_WhenSelect() const
{
    if ( EdGraphNode.IsValid() )
    {
        if ( const auto WhenSelectThisNode = Cast<IWhenSelectThisNode>(EdGraphNode.Get()) )
        {
            return FText::FromName(WhenSelectThisNode->GetWhenSelectThisNodeFunctionName());
        }
    }
    return FText::FromString(TEXT("Select Function..."));
}

const UClass* FDialogueNodeDetail::GetBlueprintGeneratedClass() const
{
    if ( EdGraphNode.IsValid() )
    {
        if ( const auto Asset = EdGraphNode->GetTypedOuter<UDialogueGraph>() )
        {
            if ( const UBlueprint* Blueprint = Asset->GetBlueprintInstance() )
            {
                return Blueprint->GeneratedClass;
            }
        }
    }
    return nullptr;
}

UEdGraph* FDialogueNodeDetail::CreateNewFunction_WhenSelect(const FName NewFunctionName) const
{
    check(EdGraphNode.IsValid())
    const auto Asset = EdGraphNode->GetTypedOuter<UDialogueGraph>();

    check(Asset)
    const auto Blueprint = Asset->GetBlueprintInstance();
    FDialogueBlueprintUtils::CreateWhenSelectedFunction(Blueprint, NewFunctionName);

    return FDialogueBlueprintUtils::FindFunctionGraph(Blueprint, NewFunctionName);
}

UEdGraph* FDialogueNodeDetail::CreateNewFunction_CanSelect(const FName NewFunctionName) const
{
    check(EdGraphNode.IsValid())
    const auto Asset = EdGraphNode->GetTypedOuter<UDialogueGraph>();

    check(Asset)
    const auto Blueprint = Asset->GetBlueprintInstance();
    FDialogueBlueprintUtils::CreateCanSelectThisNodeFunction(Blueprint, NewFunctionName);

    return FDialogueBlueprintUtils::FindFunctionGraph(Blueprint, NewFunctionName);
}

#undef LOCTEXT_NAMESPACE
