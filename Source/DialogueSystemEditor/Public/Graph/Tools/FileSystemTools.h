// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Function/FunctionExecutor.h"

/// <summary>
/// AI에서 호출할 수 있는 파일 시스템 조작 도구
/// </summary>
class DIALOGUESYSTEMEDITOR_API FWriteFileTool final : public FFunctionExecutor
{
public:
    FWriteFileTool();
    virtual ~FWriteFileTool() override;
    virtual FFunctionExecutionResult Execute(const FJsonObject& JsonObject) const override;

protected:
    virtual FFunctionDeclaration GetDeclaration_Internal() const override;
};
