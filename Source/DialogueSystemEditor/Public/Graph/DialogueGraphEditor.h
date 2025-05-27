#pragma once
#include "CoreMinimal.h"
#include "DialogueEdGraph.h"
#include "EditorUndoClient.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UDialogueGraphAsset;
class UEdGraph;
class UEdGraphNode;
class SGraphEditor;

#define LOCTEXT_NAMESPACE "DialogueGraphEditor"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// FDialogueGraphEditor
/// 
///     설명:
///         - 대화 시스템에서 사용하는 그래프를 편집하는 에디터 윈도우
///         - 에디터 창을 신규로 소환하고 그 안에 SGraphEditor 위젯을 탭으로 스폰
///         
///     TODO:
///         - 에디터 단축키 동작 콜백 작성
///         - 디테일 패널 위젯
///         - 컴파일 위젯 추가
///         
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FDialogueGraphEditor : public FWorkflowCentricApplication,
                             public FEditorUndoClient
{
#pragma region 라이프사이클 // 생성자, 소멸자, 에디터 초기화

public:
    // 생성자 
    FDialogueGraphEditor();

    // 소멸자 
    virtual ~FDialogueGraphEditor() override;

    // 에디터 초기화 
    void InitDialogueGraphEditor(const EToolkitMode::Type Mode,
                                 const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                 UDialogueGraphAsset* InDialogueGraph);
#pragma endregion

#pragma region FWorkflowCentricApplication_Implementation // 탭 구현

public:
    // Tab Spawner 등록
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
    // Tab Spawner 등록해제
    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
    // Tab Spawner들
    TSharedRef<SDockTab> GraphEditorTabSpawner(const FSpawnTabArgs& SpawnTabArgs);
    TSharedRef<SDockTab> PropertyTabSpawner(const FSpawnTabArgs& Args);
#pragma endregion

#pragma region Toolkit_Implementation

public:
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FText GetToolkitName() const override;
    virtual FText GetToolkitToolTipText() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;
#pragma endregion

#pragma region FEditorUndoClient_Implementation

public:
    virtual void PostUndo(bool bSuccess) override;
    virtual void PostRedo(bool bSuccess) override;
#pragma endregion

#pragma region 에디터_윈도우_이벤트_리스너

public:
    void OnDeleteKeyPressed();
#pragma endregion

#pragma region 그래프_에디터_위젯_이벤트_리스너

public:
    // 그래프 에디터 이벤트 설정 
    void SetupGraphEditorEvents(UDialogueEdGraph* InGraph,
                                SGraphEditor::FGraphEditorEvents& InEvents);
    // 노드 선택 변경 이벤트
    void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
    void OnDoubleClicked();
    void OnCreateComment();
#pragma endregion

#pragma region 위젯_팩토리_메서드

private:
    // 그래프 에디터 위젯 생성 
    TSharedRef<SGraphEditor> CreateGraphEditorWidget(UDialogueEdGraph* InGraph);
#pragma endregion

#pragma region 프로퍼티
    // Getter
public:
    // 현재 편집 중인 대화 그래프 에셋을 반환 
    UDialogueGraphAsset* GetDialogueGraphAsset() const { return DialogueGraphAsset; }

    // Properties
private:
    // 대화 그래프 에셋 
    UDialogueGraphAsset* DialogueGraphAsset;

    // 그래프 에디터 명령 리스트 
    TSharedPtr<FUICommandList> GraphEditorCommands;

    // 현재 포커스된 그래프 에디터 
    TWeakPtr<SGraphEditor> FocusedGraphEdPtr;

    // 속성 에디터 
    TSharedPtr<IDetailsView> PropertyEditor;
#pragma endregion
};

#undef LOCTEXT_NAMESPACE
