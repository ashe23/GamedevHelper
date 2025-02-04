// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "GdhBetRenderList.generated.h"

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<TSoftObjectPtr<ULevelSequence>> Sequences;
};
