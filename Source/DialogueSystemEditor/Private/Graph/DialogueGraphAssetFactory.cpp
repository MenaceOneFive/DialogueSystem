// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueGraphAssetFactory.h"

#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphDirector.h"
#include "Graph/DialogueGraphEditor.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "DialogueAsset"

#pragma region AssetFactory

UDialogueGraphAssetFactory::UDialogueGraphAssetFactory(const FObjectInitializer& ObjectInitializer): UFactory(ObjectInitializer)
{
    bCreateNew     = true;
    SupportedClass = UDialogueGraph::StaticClass();
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
    UDialogueGraph* DialogueAsset = NewObject<UDialogueGraph>(InParent, InClass, InName, Flags);
    DialogueAsset->AssetUserData  = NewObject<UDialogueEdGraphAssetData>(DialogueAsset);

    // 대화 에셋의 블루프린트를 생성
    CreateBlueprintForAsset(DialogueAsset);

    // 에셋에 포함되는 에디터타임 그래프를 초기화
    const auto AssetUserData               = Cast<UDialogueEdGraphAssetData>(DialogueAsset->AssetUserData);
    AssetUserData->DialogueEdGraph         = NewObject<UDialogueEdGraph>(DialogueAsset->AssetUserData, TEXT("DialogueGraph"), RF_Transactional);
    AssetUserData->DialogueEdGraph->Schema = UDialogueGraphSchema::StaticClass();

    // 에디터 타임에 기본 노드를 추가
    UDialogueEdGraphStartNode* StartNode = NewObject<UDialogueEdGraphStartNode>(AssetUserData->DialogueEdGraph);
    StartNode->AllocateDefaultPins();
    AssetUserData->DialogueEdGraph->AddNode(StartNode);
    return DialogueAsset;
}

UBlueprint* UDialogueGraphAssetFactory::CreateBlueprintForAsset(UDialogueGraph* GraphAsset) const
{
    UBlueprint* BlueprintInstance = GraphAsset->GetBlueprintInstance();
    if ( BlueprintInstance )
    {
        ensureMsgf(!BlueprintInstance, TEXT("Should not call %s when one already exists"), __FUNCTION__);
        return BlueprintInstance;
    }

    BlueprintInstance = FKismetEditorUtilities::CreateBlueprint(UDialogueGraphDirector::StaticClass(),
                                                                GraphAsset,
                                                                FName(*GraphAsset->GetDirectorBlueprintName()),
                                                                BPTYPE_Normal,
                                                                UBlueprint::StaticClass(),
                                                                UBlueprintGeneratedClass::StaticClass());

    check(BlueprintInstance);

    GraphAsset->SetBlueprintInstance(BlueprintInstance);

    // BlueprintInstance->OnCompiled().AddLambda([Asset = TObjectPtr<UDialogueGraphAsset>(GraphAsset)](UBlueprint* NewBlueprint)
    // {
    //     if ( Asset && Asset->IsValidLowLevel() )
    //     {
    //         Asset->SetBlueprintInstance(NewBlueprint);
    //     }
    // });

    return BlueprintInstance;
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
    return UDialogueGraph::StaticClass();
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
        if ( UDialogueGraph* DialogueGraph = Cast<UDialogueGraph>(Object) )
        {
            // 에디터 생성
            const TSharedRef<FDialogueGraphEditor> NewDialogueGraphEditor = MakeShared<FDialogueGraphEditor>();

            // 에디터 초기화
            NewDialogueGraphEditor->InitDialogueGraphEditor(Mode, EditWithinLevelEditor, DialogueGraph);
        }
    }
}
#pragma endregion

#undef LOCTEXT_NAMESPACE
