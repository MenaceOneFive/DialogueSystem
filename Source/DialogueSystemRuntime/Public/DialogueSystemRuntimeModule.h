// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API FDialogueSystemRuntimeModule : public IModuleInterface
{
public:
	virtual void ShutdownModule() override;
	virtual void StartupModule() override;
};
