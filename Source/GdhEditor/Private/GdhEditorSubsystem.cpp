// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorSubsystem.h"
#include "GdhEditor.h"
// Engine Headers
#include "GdhRenderingSettings.h"
#include "LevelEditorSubsystem.h"
#include "LevelSequence.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Subsystems/UnrealEditorSubsystem.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegisterContextMenuActions();
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGdhSubsystem::RenderLevelSequence(const TSoftObjectPtr<ULevelSequence> LevelSequence, const TSoftObjectPtr<UWorld> Map, const bool bCreateVideo)
{
	if (!GEditor) return;

	const ULevelSequence* Sequence = LevelSequence.LoadSynchronous();
	if (!Sequence) return;

	const UWorld* MapAsset = Map.LoadSynchronous();
	if (!MapAsset)
	{
		MapAsset = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
	}

	if (!MapAsset) return;

	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue) return;

	CustomQueue->DeleteAllJobs();

	UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
	if (!Job) return;

	UGdhRenderingSettings* RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return;

	RenderingSettings->Validate();
	if (!RenderingSettings->IsValid())
	{
		UE_LOG(LogGdhEditor, Error, TEXT("Invalid settings: %s"), *RenderingSettings->GetErrorMsg());
		return;
	}

	Job->SetConfiguration(RenderingSettings->GetMasterConfig(Sequence));
	Job->Map = MapAsset;
	Job->JobName = Sequence->GetName();
	Job->SetSequence(Sequence);

	UMoviePipelineExecutorBase* Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	if (!Executor) return;

	Executor->OnExecutorFinished().AddLambda([](const UMoviePipelineExecutorBase* ExecutorBase, const bool bSuccess)
	{
		if (!bSuccess)
		{
			UE_LOG(LogGdhEditor, Error, TEXT("Error occured when rendering images"));
			return;
		}

		UE_LOG(LogGdhEditor, Warning, TEXT("Rendering images finished successfully"));
	});
}

void UGdhSubsystem::RegisterContextMenuActions() const
{
	// todo:ashe23
}
