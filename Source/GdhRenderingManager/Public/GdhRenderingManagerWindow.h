// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhRenderingQueueSettings;

class GDHRENDERINGMANAGER_API SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
private:
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhRenderingQueueSettings* RenderingQueueSettings = nullptr;
};