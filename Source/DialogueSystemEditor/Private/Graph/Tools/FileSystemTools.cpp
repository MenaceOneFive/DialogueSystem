// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Tools/FileSystemTools.h"
#include "Function/FunctionDeclarationBuilder.h"


FWriteFileTool::FWriteFileTool()
{
}

FWriteFileTool::~FWriteFileTool()
{
}

FFunctionExecutionResult FWriteFileTool::Execute(const FJsonObject& JsonObject) const
{
    const FString Content  = JsonObject.GetStringField(TEXT("content"));
    const FString FileName = JsonObject.GetStringField(TEXT("fileName"));

    UE_LOG(LogTemp, Log, TEXT("file : %s content %s"), *FileName, *Content)

    FFunctionExecutionResult ExecutionResult;
    ExecutionResult.SetResponseType(EResponseType::SUCCESS_SEND_REPLY);
    const TSharedPtr<FJsonObject> ResponseObject = MakeShared<FJsonObject>();
    ResponseObject->SetStringField(TEXT("message"), TEXT("성공적으로 파일을 저장했습니다."));

    ExecutionResult.SetResponseObject(ResponseObject);
    return ExecutionResult;
}

FFunctionDeclaration FWriteFileTool::GetDeclaration_Internal() const
{
    static TUniquePtr<FFunctionDeclaration> FunctionDeclarationHolder;
    if ( !FunctionDeclarationHolder.IsValid() )
    {
        FFunctionDeclarationBuilder Builder = FFunctionDeclarationBuilder();
        const FString Description           =
                FString::Format(TEXT(R"(Write a file to disk
                - content : {0}
                - fileName : {1}
                - response : {2} )"), {*FString(TEXT("저장할 문자열 컨텐츠")), *FString(TEXT("저장할 파일명")), *FString(TEXT("파일의 저장 결과"))});

        Builder.SetName("writeToFile")
               .SetDescription(Description)
               .SetParameterSchema(FJsonSchemaBuilder()
                                   .AddStringProperty("content", TEXT("저장할 문자열 컨텐츠"))
                                   .AddStringProperty("fileName", TEXT("저장할 파일명")))
               .SetResponseSchema(FJsonSchemaBuilder()
                                  .AddStringProperty("response", TEXT("파일의 저장결과")));
        FunctionDeclarationHolder = MakeUnique<FFunctionDeclaration>(Builder.Build());
    }
    return *FunctionDeclarationHolder.Get();
}
