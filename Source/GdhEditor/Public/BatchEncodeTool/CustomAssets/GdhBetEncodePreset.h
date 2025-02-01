// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhBetEncodePreset.generated.h"

UCLASS(BlueprintType)
class GDHEDITOR_API UGdhBetEncodePreset : public UObject
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encode")
	FName PresetName;

	// FFmpeg command to use when encoding images to videos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encode")
	TArray<FString> EncodeCmd;
};
