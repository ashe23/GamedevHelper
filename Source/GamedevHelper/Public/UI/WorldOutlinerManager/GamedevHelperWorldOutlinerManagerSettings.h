// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperWorldOutlinerManagerSettings.generated.h"

UCLASS(Transient)
class UGamedevHelperWorldOutlinerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperWorldOutlinerSettings();

	FName GetFolderNameByActor(const AActor* Actor);

	UPROPERTY(EditAnywhere, Category="WorldOutlinerManagerSettings", meta=(ToolTip="Move actors whos class is not specified, to 'Unsorted' folder"))
	bool bMoveToUnsorted = true;

	// UPROPERTY(EditAnywhere, Category="WorldOutlinerManagerSettings")
	// bool bFixAssetNames = false;
	
	UPROPERTY(EditAnywhere, Category="WorldOutlinerManagerSettings")
	TMap<UClass*, FName> ActorFolderMappings;
private:
	void SetDefaultSettings();
};
