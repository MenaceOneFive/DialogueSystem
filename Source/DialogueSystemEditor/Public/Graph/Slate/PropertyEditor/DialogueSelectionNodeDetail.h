#pragma once
#include "IDetailCustomization.h"
#include "Graph/Slate/DialogueNodeDetail.h"

class FDialogueSelectionNodeDetail final : public FDialogueNodeDetail
{
public: // 정적 생성자
    static TSharedRef<IDetailCustomization> MakeInstance();
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    void CreateMulticastEventCustomization(IDetailLayoutBuilder& DetailBuilder);
};
