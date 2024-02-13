// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class GDHEDITOR_API FGdhStyles
{
public:
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();
	static FSlateIcon GetIcon(const FString& IconSpecifier);
	static const FSlateBrush* GetIconBrush(const FString& Specifier);
	static FSlateColor GetColor(const FString& Specifier);
	static FSlateFontInfo GetFont(const FString& FontType, const uint32 FontSize);

private:
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};
