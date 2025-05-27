// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueGraphAsset.h"

#include "NiagaraNodeInput.h"
#include "Algo/Count.h"
#include "Graph/DialogueEdGraphNodes.h"
#include "Graph/DialogueGraphEditor.h"

#define LOCTEXT_NAMESPACE "DialogueAsset"

#pragma region AssetFactory
UDialogueGraphAssetFactory::UDialogueGraphAssetFactory(const FObjectInitializer& ObjectInitializer): UFactory(ObjectInitializer)
{
    bCreateNew     = true;
    SupportedClass = UDialogueGraphAsset::StaticClass();
}

FText UDialogueGraphAssetFactory::GetDisplayName() const
{
    return LOCTEXT("DialogueAssetTypeActions_TestPlugin", "Dialogue Asset");
}

UObject* UDialogueGraphAssetFactory::FactoryCreateNew(UClass* InClass,
                                                      UObject* InParent,
                                                      FName InName,
                                                      EObjectFlags Flags,
                                                      UObject* Context,
                                                      FFeedbackContext* Warn)
{
    UDialogueGraphAsset* DialogueAsset = NewObject<UDialogueGraphAsset>(InParent, InClass, InName, Flags);
    DialogueAsset->InitializeAsset();
    return DialogueAsset;
}

bool UDialogueGraphAssetFactory::ShouldShowInNewMenu() const
{
    return true;
}

uint32 UDialogueGraphAssetFactory::GetMenuCategories() const
{
    return FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().FindAdvancedAssetCategory(DialogueAsset::CategoryBase);
}
#pragma endregion

#pragma region AssetAction
UClass* FAssetTypeActions_DialogueGraph::GetSupportedClass() const
{
    return UDialogueGraphAsset::StaticClass();
}

uint32 FAssetTypeActions_DialogueGraph::GetCategories()
{
    return FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().FindAdvancedAssetCategory(DialogueAsset::CategoryBase);
}

void FAssetTypeActions_DialogueGraph::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                      TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    // 편집기에서 열 대화 그래프 에셋 선택
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ?
                                        EToolkitMode::WorldCentric :
                                        EToolkitMode::Standalone;

    for ( UObject* Object : InObjects )
    {
        if ( UDialogueGraphAsset* DialogueGraph = Cast<UDialogueGraphAsset>(Object) )
        {
            // 에디터 생성
            const TSharedRef<FDialogueGraphEditor> NewDialogueGraphEditor = MakeShared<FDialogueGraphEditor>();

            // 에디터 초기화
            NewDialogueGraphEditor->InitDialogueGraphEditor(Mode, EditWithinLevelEditor, DialogueGraph);
        }
    }
}
#pragma endregion

#pragma region AssetItSelf
void UDialogueGraphAsset::InitializeAsset()
{
    if ( DialogueGraph == nullptr )
    {
        DialogueGraph         = NewObject<UDialogueEdGraph>(this, TEXT("DialogueGraph"), RF_Transactional);
        DialogueGraph->Schema = UDialogueGraphSchema::StaticClass();

        UDialogueEdGraphStartNode* StartNode = NewObject<UDialogueEdGraphStartNode>(DialogueGraph);
        DialogueGraph->AddNode(StartNode);
    }
}

const FString& UDialogueGraphAsset::GetDescription() const
{
    return Description;
}

uint32 UDialogueGraphAsset::CountNode() const
{
    if ( DialogueGraph == nullptr )
    {
        return 0;
    }
    return DialogueGraph->Nodes.Num();
}

uint32 UDialogueGraphAsset::CountSelectNode() const
{
    if ( DialogueGraph == nullptr )
    {
        return 0;
    }
    return Algo::CountIf(DialogueGraph->Nodes, [](const UEdGraphNode* Node)
    {
        return Node->IsA<UDialogueEdGraphSelectNode>();
    });
}

uint32 UDialogueGraphAsset::CountLineNode() const
{
    if ( DialogueGraph == nullptr )
    {
        return 0;
    }
    return Algo::CountIf(DialogueGraph->Nodes, [](const UEdGraphNode* Node)
    {
        return Node->IsA<UDialogueEdGraphDialogueLineNode>();
    });
}
#pragma endregion
#undef LOCTEXT_NAMESPACE
