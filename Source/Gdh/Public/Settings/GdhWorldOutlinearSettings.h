// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhWorldOutlinearSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="World Outliner Settings")
class UGdhWorldOutlinearSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGdhWorldOutlinearSettings();

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="World Outliner Settings", meta=(ToolTip="Move actors whos class is not specified, to 'Unsorted' folder"))
	bool bMoveToUnsorted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="World Outliner Settings", meta=(EditCondition="bMoveToUnsorted"))
	FString UnsortedFolderName = TEXT("Unsorted");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="World Outliner Settings")
	TMap<TSoftClassPtr<UObject>, FString> Mappings;
};
