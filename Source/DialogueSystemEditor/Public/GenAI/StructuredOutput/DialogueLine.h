#pragma once
#include "StructuredOutput/StructuredOutputDeclaration.h"

struct FDialogueLineOutputDeclaration final : public FStructuredOutputDeclaration
{
    FDialogueLineOutputDeclaration();

    virtual FStructuredOutputBuilder GetOutputDeclarationBuilder(const EStructuredOutputBuilderContext Context) const override
    {
        FStructuredOutputBuilder SchemaRootBuilder;
        SchemaRootBuilder.SetContext(Context)
                         .AddRequiredStringProperty("character_name", TEXT("캐릭터 이름"))
                         .AddRequiredStringProperty("dialogue_line", TEXT("캐릭터가 말한 대사"))
                         .AddRequiredStringProperty("emotion", TEXT("캐릭터가 dialogue_line을 말할 때의 감정"));
        FStructuredOutputBuilder ArrayBuilder;
        ArrayBuilder.SetContext(Context)
                    .AddRequiredArrayProperty("dialogue_lines", &SchemaRootBuilder);
        return ArrayBuilder;
    }
};

inline FDialogueLineOutputDeclaration::FDialogueLineOutputDeclaration()
{
    Name        = "DialogueLines";
    Description = TEXT("캐릭터가 말할 대사");
}
