// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "GdhStructs.h"
#include "GdhBetRenderList.generated.h"

class UMoviePipelineMasterConfig;

DECLARE_MULTICAST_DELEGATE(FOnRenderListChanged)

/**
 * UGdhBetRenderList
 *
 * A custom Unreal Engine asset type designed to store and manage a batch of Level Sequences
 * for rendering purposes. This asset allows users to organize multiple sequences and execute batch
 * rendering efficiently. It is primarily used to streamline the process of generating rendered
 * images for cinematic sequences.
 *
 * Features:
 * - Holds a list of ULevelSequence assets.
 * - Displays sequence statistics (e.g., duration, frame count).
 * - Designed for batch rendering workflows.
 * - Integrates with Unreal Engine's rendering pipeline.
 */
UCLASS(BlueprintType)
class GDHEDITOR_API UGdhBetRenderList : public UObject
{
public:

	GENERATED_BODY()

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Output directory for rendered images and encoded videos.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render", DisplayName = "OutputDir")
	FDirectoryPath DirOutput;

	// Which level to use when rendering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
	TSoftObjectPtr<UWorld> World;

	// Main Movie Render Queue settings that will be used when rendering level sequences.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
	TSoftObjectPtr<UMoviePipelineMasterConfig> RenderSettings;

	// List of sequences that should be rendered and encoded
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Render")
	TSet<TSoftObjectPtr<ULevelSequence>> Sequences;

	// Encode presets that should be used when encoding videos
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Encode")
	TMap<FString, FGdhEncodePreset> EncodePresets;

	// Optional audio track name to path mapping, if we want to embed audio in final video
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Encode")
	TMap<FString, FFilePath> AudioTracks;

	FOnRenderListChanged OnRenderListChanged;
};
