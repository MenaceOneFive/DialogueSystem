#pragma once
#include <CoreMinimal.h>
#include <EditorUndoClient.h>
#include <Graph/DialogueEdGraph.h>
#include <Toolkits/AssetEditorToolkit.h>

#include "WorkflowOrientedApp/WorkflowCentricApplication.h"


class UDialogueGraph;
class UEdGraph;
class UEdGraphNode;
class SGraphEditor;
class FUICommandList; // FUICommandList 전방 선언 추가

#define LOCTEXT_NAMESPACE "DialogueGraphEditor"

/// <summary>
/// FDialogueGraphEditor
///
/// 대화 시스템에서 사용하는 그래프를 편집하는 에디터 윈도우
/// 에디터 창을 신규로 소환하고 그 안에 SGraphEditor 위젯을 탭으로 스폰
/// <br> 
/// TODO:
/// - 에디터 단축키 동작 콜백 작성
/// - 디테일 패널 위젯
/// - 컴파일 위젯 추가
/// </summary>
class FDialogueGraphEditor final : public FWorkflowCentricApplication,
                                   public FEditorUndoClient
{
public: // 생성자, 소멸자, 초기화

    // 생성자 
    FDialogueGraphEditor();

    // 소멸자 
    virtual ~FDialogueGraphEditor() override;

    // 에디터 초기화 
    void InitDialogueGraphEditor(const EToolkitMode::Type Mode,
                                 const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                 UDialogueGraph* InDialogueGraph);

public: // FWorkflowCentricApplication 상속

    // Tab Spawner 등록
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

    // Tab Spawner 등록해제
    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

    // Tab Spawner들

    // GraphEditor : 그래프 패널
    TSharedRef<SDockTab> GraphEditorTabSpawner(const FSpawnTabArgs& SpawnTabArgs);

    // Property : 속성 패널
    TSharedRef<SDockTab> PropertyTabSpawner(const FSpawnTabArgs& Args);

public: // AssetEditorToolkit 상속
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FText GetToolkitName() const override;
    virtual FText GetToolkitToolTipText() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;

public: // EditorUndoClient 상속
    virtual void PostUndo(bool bSuccess) override;
    virtual void PostRedo(bool bSuccess) override;


    // 그래프 에디터 이벤트 설정 
    void SetupGraphEditorEvents(UDialogueEdGraph* InGraph,
                                SGraphEditor::FGraphEditorEvents& InEvents);

public: // 이벤트 리스너

    // 노드 선택 변경 이벤트
    void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection) const;

    void OnDeleteKeyPressed() const;

    void OnDoubleClicked() const;

    void OnCreateComment() const;

protected: // 저장관련

    void GenerateRuntimeGraph() const;
    virtual void SaveAsset_Execute() override;

public: // Getter

    // 현재 편집 중인 대화 그래프 에셋을 반환 
    UDialogueGraph* GetDialogueGraphAsset() const { return DialogueGraphAsset; }

private: // Helper Functions

    void OnBlueprintRecompiled(UBlueprint* Blueprint,
                               UDialogueGraph* Asset);

    // 그래프 에디터 위젯 생성 
    TSharedRef<SGraphEditor> CreateGraphEditorWidget(UDialogueEdGraph* InGraph);

    /// <summary>
    /// 선택된 노드를 삭제하는 명령어 실행 함수
    /// </summary>
    void DeleteSelectedNode() const;

    /// <summary>
    /// 선택된 노드를 삭제할 수 있는지 확인하는 함수
    /// </summary>
    /// <returns>삭제 가능 여부</returns>
    bool CanExecuteDeleteSelectedNode() const;

    /// <summary>
    /// 선택된 노드를 복사합니다.
    /// </summary>
    void CopySelectedNodes() const;

    /// <summary>
    /// 노드를 복사할 수 있는지 확인합니다.
    /// </summary>
    /// <returns>복사 가능 여부</returns>
    bool CanCopyNodes() const;

    /// <summary>
    /// 노드를 붙여넣습니다.
    /// </summary>
    void PasteNodes() const;

    /// <summary>
    /// 노드를 붙여넣을 수 있는지 확인합니다.
    /// </summary>
    /// <returns>붙여넣기 가능 여부</returns>
    bool CanPasteNodes() const;

    /// <summary>
    /// 선택된 노드를 잘라냅니다.
    /// </summary>
    void CutSelectedNodes() const;

    /// <summary>
    /// 노드를 잘라낼 수 있는지 확인합니다.
    /// </summary>
    /// <returns>잘라내기 가능 여부</returns>
    bool CanCutNodes() const;

    /// <summary>
    /// 선택된 노드를 복제합니다.
    /// </summary>
    void DuplicateNodes() const;

    /// <summary>
    /// 노드를 복제할 수 있는지 확인합니다.
    /// </summary>
    /// <returns>복제 가능 여부</returns>
    bool CanDuplicateNodes() const;

    /// <summary>
    /// 선택된 노드의 모든 연결을 삭제하는 명령어 실행 함수
    /// </summary>
    void DeleteAllNodeConnection() const;

    /// <summary>
    /// 선택된 노드의 모든 연결을 삭제할 수 있는지 확인하는 함수
    /// </summary>
    /// <returns>연결 삭제 가능 여부</returns>
    bool CanDeleteAllNodeConnection() const;

    void RemoveSelectedNodesFrom(const TSharedPtr<SGraphEditor>& GraphEditor) const;

    /// <summary>
    /// 현재 선택된 노드들을 가져옵니다.
    /// </summary>
    /// <returns>선택된 노드 Set</returns>
    TSet<UObject*> GetSelectedNodes() const;

public: // 인-에디터 자막 관련

    /// <summary>
    /// 자막을 에디터 하단에 표시합니다.
    /// </summary>
    /// <param name="SubtitleText">표시할 자막</param>
    void SetSubtitleText(const FString& SubtitleText) const;

    /// <summary>
    /// 자막을 표시합니다.
    /// </summary>
    void ShowSubtitle() const;

    /// <summary>
    /// 자막을 숨깁니다.
    /// </summary>
    void HideSubtitle() const;

private: // 프로퍼티

    // 대화 그래프 에셋 
    UDialogueGraph* DialogueGraphAsset;

    // 그래프 에디터 명령 리스트 
    TSharedPtr<FUICommandList> GraphEditorCommands;

    // 현재 포커스된 그래프 에디터 
    TWeakPtr<SGraphEditor> FocusedGraphEdPtr;

    // 속성 에디터 
    TSharedPtr<IDetailsView> PropertyEditor;

    SGraphEditor::FOnCreateNodeOrPinMenu OnCreateNodeOrPinMenuDelegate;

    TSharedPtr<STextBlock> Tb_CurrentLine;

    SOverlay::FOverlaySlot* CurrentLineOverlay = nullptr;
    FDelegateHandle OnCompiledHandle;
    FDelegateHandle OnChangedHandle;
};

class FDialogueGraphEditorUtils
{
public:
    /// <summary>
    /// 노드를 보유하는 에셋을 반환합니다.
    /// 만약 nullptr를 반환한다면 
    /// </summary>
    /// <param name="Node">그래프 에디터 노드</param>
    /// <returns>노드를 보유하고 있는 에셋</returns>
    TObjectPtr<UDialogueGraph> GetDialogueAsset(const UDialogueEdGraphNode* Node) const;

    static void OnBlueprintChanged(UBlueprint* Blueprint);
};

#undef LOCTEXT_NAMESPACE
