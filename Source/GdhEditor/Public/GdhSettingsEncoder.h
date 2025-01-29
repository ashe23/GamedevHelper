// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhSettingsEncoder.generated.h"

UCLASS(BlueprintType)
class GDHEDITOR_API UGdhSettingsEncoder : public UObject
{
	GENERATED_BODY()
  public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gdh")
	TArray<FString> Cmd;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gdh")
	FString CmdVisualizer;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Gdh")
	void VisualizerUpdate();
};
