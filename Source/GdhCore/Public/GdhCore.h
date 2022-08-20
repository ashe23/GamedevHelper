// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

namespace GdhCoreConstants
{
	static const FName ModuleName{TEXT("GdhCore")};
}

// This module responsible for rendering images from LevelSequences or MoviePipelineQueue assets, and encoding them into video files
class FGdhCore : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;
};