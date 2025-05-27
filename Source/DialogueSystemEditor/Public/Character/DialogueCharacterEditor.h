#pragma once
#include "CoreMinimal.h"
#include "EditorUndoClient.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UDialogueCharacterAsset;
class SDockTab;
class FSpawnTabArgs;
class IDetailsView;

#define LOCTEXT_NAMESPACE "DialogueCharacterEditor"

class FDialogueCharacterEditor final : public FWorkflowCentricApplication,
                                       public FEditorUndoClient
{
public:
    FDialogueCharacterEditor();
    virtual ~FDialogueCharacterEditor() override;

    void InitDialogueCharacterEditor(const EToolkitMode::Type Mode,
                                     const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                     UDialogueCharacterAsset* InDialogueCharacterAsset);

    // Tab 등록/해제
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

    // 탭 스포너 (PropertyTab)
    TSharedRef<class SDockTab> PropertyTabSpawner(const class FSpawnTabArgs& Args);

    // 기본 정보 반환
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FText GetToolkitName() const override;
    virtual FText GetToolkitToolTipText() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;

protected:
    UDialogueCharacterAsset* DialogueCharacterAsset;

private:
    TSharedPtr<class IDetailsView> PropertyEditor;
};

#undef LOCTEXT_NAMESPACE
