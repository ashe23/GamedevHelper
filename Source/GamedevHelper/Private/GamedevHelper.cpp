// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelper.h"
// Engine Headers
#include "UnrealEdMisc.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

class FGamedevHelper : public IGamedevHelper
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


void FGamedevHelper::StartupModule() {}

void FGamedevHelper::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelper, GamedevHelper)
DEFINE_LOG_CATEGORY(LogGamedevHelper);
