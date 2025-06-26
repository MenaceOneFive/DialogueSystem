// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/// <summary>
/// 핀간 연결이 적절한지 검사하는 정책들을 정의
/// </summary>
class FPinConnectionPolicy
{
public:
    virtual ~FPinConnectionPolicy() = default;
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B);
};

/// <summary>
/// 핀의 입력/출력에 상관 없이 양 끝단이 같은 노드, 즉 자기 자신에 대한 연결을 막는 정책
/// 만약 재귀적 연결을 허용하고 싶다면 노드의 bAllowRecursiveConnection를 true로 설정해야 한다.
/// </summary>
class FRecursivePinConnectionPolicy final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};

/// <summary>
/// 두 핀의 방향은 같을 수 없다.
/// </summary>
class FDirectionPinConnectionPolicy final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};

/// <summary>
/// 출력핀은 하나의 커넥션만 가질 수 있다.
/// </summary>
class FOutputPinConnectionPolicy final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};

/// <summary>
/// 선택 노드는 다른 노드에 1번만 연결할 수 있다.
/// </summary>
class FSelectNodeOutputPinDupConnection final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};
