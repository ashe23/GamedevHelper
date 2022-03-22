// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GamedevHelperEditorStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedPtr<FSlateStyleSet> FGamedevHelperEditorStyle::StyleInstance = nullptr;

void FGamedevHelperEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGamedevHelperEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

void FGamedevHelperEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FGamedevHelperEditorStyle::Get()
{
	return *StyleInstance;
}

FName FGamedevHelperEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("GamedevHelperStyle"));
	return StyleSetName;
}

const FSlateBrush* FGamedevHelperEditorStyle::GetIcon(const FString& Specifier)
{
	return Get().GetBrush(*Specifier);
}

FSlateColor FGamedevHelperEditorStyle::GetColor(const FString& Specifier)
{
	return Get().GetSlateColor(*Specifier);
}

TSharedRef<FSlateStyleSet> FGamedevHelperEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("GamedevHelperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("GamedevHelper")->GetBaseDir() / TEXT("Resources"));

	// fonts light
	Style->Set("GamedevHelper.Font.Light30", FCoreStyle::GetDefaultFontStyle("Light", 30));
	Style->Set("GamedevHelper.Font.Light20", FCoreStyle::GetDefaultFontStyle("Light", 20));
	Style->Set("GamedevHelper.Font.Light15", FCoreStyle::GetDefaultFontStyle("Light", 15));
	Style->Set("GamedevHelper.Font.Light10", FCoreStyle::GetDefaultFontStyle("Light", 10));
	Style->Set("GamedevHelper.Font.Light8", FCoreStyle::GetDefaultFontStyle("Light", 8));

	// fonts bold
	Style->Set("GamedevHelper.Font.Bold30", FCoreStyle::GetDefaultFontStyle("Bold", 30));
	Style->Set("GamedevHelper.Font.Bold20", FCoreStyle::GetDefaultFontStyle("Bold", 20));
	Style->Set("GamedevHelper.Font.Bold15", FCoreStyle::GetDefaultFontStyle("Bold", 15));
	Style->Set("GamedevHelper.Font.Bold10", FCoreStyle::GetDefaultFontStyle("Bold", 10));
	Style->Set("GamedevHelper.Font.Bold8", FCoreStyle::GetDefaultFontStyle("Bold", 8));

	// icons
	Style->Set("GamedevHelper.Icon16", new IMAGE_BRUSH(TEXT("Icon16"), Icon16x16));
	Style->Set("GamedevHelper.Icon.ArrowRight.16", new IMAGE_BRUSH(TEXT("IconArrowRight512"), Icon16x16));
	Style->Set("GamedevHelper.Icon.BG.16", new IMAGE_BRUSH(TEXT("IconBG16"), Icon16x16));
	Style->Set("GamedevHelper.Cmd_AssetNamingManagerWindow", new IMAGE_BRUSH(TEXT("IconRename64"), Icon40x40));
	Style->Set("GamedevHelper.Cmd_AssetNamingManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconRename64"), Icon20x20));
	Style->Set("GamedevHelper.Cmd_WorldOutlinerManagerWindow", new IMAGE_BRUSH(TEXT("IconOrganize64"), Icon40x40));
	Style->Set("GamedevHelper.Cmd_WorldOutlinerManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconOrganize64"), Icon20x20));

	// colors
	Style->Set("GamedevHelper.Color.Red", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#C62828"))}});
	Style->Set("GamedevHelper.Color.Green", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#2E7D32"))}});
	Style->Set("GamedevHelper.Color.Grey", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#616161"))}});


	return Style;
}
