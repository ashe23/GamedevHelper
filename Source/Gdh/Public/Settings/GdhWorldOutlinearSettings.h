// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhWorldOutlinearSettings.generated.h"

DECLARE_DELEGATE(FGdhWorldOutlinearSettingsOnChangeDelegate);

UCLASS(Config = EditorPerProjectUserSettings, meta=(DisplayName="World Outlinear Settings"))
class UGdhWorldOutlinearSettings final : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UGdhWorldOutlinearSettings();

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
	virtual FString GetDesc() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Organize by Type", meta = (ToolTip = "Organize all actors by their class type"))
	bool bOrganizeByType = false;

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Move To Unsorted",
		meta = (ToolTip = "Move actors whos class is not specified, to 'Unsorted' folder", EditCondition = "!bOrganizeByType"))
	bool bMoveToUnsorted = true;

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Unsorted folder name",
		meta = (ToolTip = "Unsorted folder name. Default 'Unsorted'", EditCondition = "bMoveToUnsorted && !bOrganizeByType"))
	FString UnsortedFolderName = TEXT("Unsorted");

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "World outliner mappings",
		meta = (ToolTip = "Actor class and folder name mappings", EditCondition = "!bOrganizeByType"))
	TMap<UClass*, FName> Mappings;

	// UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Folders To Ignore", meta = (ToolTip = "Folders whose actors wont be touched"))
	// TSet<FName> IgnoredFolders;

	FGdhWorldOutlinearSettingsOnChangeDelegate& OnChange();
private:
	FGdhWorldOutlinearSettingsOnChangeDelegate OnChangeDelegate;;
};
