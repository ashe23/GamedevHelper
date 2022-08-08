// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevHelperRendererLibrary.generated.h"

UCLASS()
class UGamedevHelperRendererLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category= "GamedevHelper|Renderer")
	static void RenderLevelSequence(const TSoftObjectPtr<ULevelSequence> SequencePath, const TSoftObjectPtr<UWorld> MapPath);
};