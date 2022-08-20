// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEditorSubsystem.generated.h"

class ULevelSequence;

UCLASS(DisplayName="GamedevHelper Subsystem")
class UGdhSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	UFUNCTION(BlueprintCallable, Category="GDH|RenderingLibrary", meta=(ToolTip="Renders specified LevelSequence using MovieRender and Gdh Rendering Settings. If Map not specified, will use current Editor Level"))
	static void RenderLevelSequence(const TSoftObjectPtr<ULevelSequence> LevelSequence, const TSoftObjectPtr<UWorld> Map, const bool bCreateVideo);
	
private:
	void RegisterContextMenuActions() const;
};