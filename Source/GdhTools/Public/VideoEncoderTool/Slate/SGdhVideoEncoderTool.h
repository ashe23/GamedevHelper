// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UMoviePipelineExecutorBase;
class UGdhVideoEncoderToolSettings;

class SGdhVideoEncoderTool final : public SCompoundWidget
{
  public:

	SLATE_BEGIN_ARGS(SGdhVideoEncoderTool) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

  private:

	void CmdsRegister();
	void OnRefreshPipelines();	 // TODO:ashe23 rename later
	void OnRenderFinished(UMoviePipelineExecutorBase*, bool);
	TSharedRef<SWidget> CreateToolbarMain() const;

	TSharedPtr<FUICommandList> Cmds;
	TWeakObjectPtr<UGdhVideoEncoderToolSettings> VideoEncoderToolSettings;
	// FProcHandle ProcessHandle;
	TArray<FString> EncodeCmds;
};
