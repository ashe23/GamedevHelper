// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "Styling/SlateStyle.h"

class FGamedevHelperEditorStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();
	static const FSlateBrush* GetIcon(const FString& Specifier);
	static FSlateColor GetColor(const FString& Specifier);
	static FString GetIconByStatus(const EGamedevHelperRendererStatus State);
	static FLinearColor GetColorByStatus(const EGamedevHelperRendererStatus State);
private:
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};
