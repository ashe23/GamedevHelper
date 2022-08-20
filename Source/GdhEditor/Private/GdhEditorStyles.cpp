// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorStyles.h"
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

TSharedPtr<FSlateStyleSet> FGdhEditorStyle::StyleInstance = nullptr;

void FGdhEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGdhEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

void FGdhEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FGdhEditorStyle::Get()
{
	return *StyleInstance;
}

FName FGdhEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("GamedevHelperStyle"));
	return StyleSetName;
}

const FSlateBrush* FGdhEditorStyle::GetIcon(const FString& Specifier)
{
	return Get().GetBrush(*Specifier);
}

FSlateColor FGdhEditorStyle::GetColor(const FString& Specifier)
{
	return Get().GetSlateColor(*Specifier);
}

FString FGdhEditorStyle::GetIconByStatus(const EGdhGenericStatus Status)
{
	if (Status == EGdhGenericStatus::OK)
	{
		return TEXT("GamedevHelper.Icon.Check20");
	}

	if (Status == EGdhGenericStatus::Warning)
	{
		return TEXT("GamedevHelper.Icon.Warning20");
	}

	return TEXT("GamedevHelper.Icon.Cross20");
}

FLinearColor FGdhEditorStyle::GetColorByStatus(const EGdhGenericStatus Status)
{
	if (Status == EGdhGenericStatus::OK)
	{
		return FLinearColor::Green;
	}

	if (Status == EGdhGenericStatus::Warning)
	{
		return FLinearColor::Yellow;
	}

	return FLinearColor::Red;
}

TSharedRef<FSlateStyleSet> FGdhEditorStyle::Create()
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
	Style->Set("GamedevHelper.Icon.Check20", new IMAGE_BRUSH(TEXT("IconCheck128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.Cross20", new IMAGE_BRUSH(TEXT("IconCross128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.Warning20", new IMAGE_BRUSH(TEXT("IconWarning128"), Icon20x20));
	// Style->Set("GamedevHelper.Cmd_AssetNamingManagerWindow", new IMAGE_BRUSH(TEXT("IconRename64"), Icon40x40));
	// Style->Set("GamedevHelper.Cmd_AssetNamingManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconRename64"), Icon20x20));
	// Style->Set("GamedevHelper.Cmd_WorldOutlinerManagerWindow", new IMAGE_BRUSH(TEXT("IconOrganize64"), Icon40x40));
	// Style->Set("GamedevHelper.Cmd_WorldOutlinerManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconOrganize64"), Icon20x20));
	Style->Set("GamedevHelper.Cmd_RestartEditor", new IMAGE_BRUSH(TEXT("IconRestart64"), Icon40x40));
	Style->Set("GamedevHelper.Cmd_RestartEditor.Small", new IMAGE_BRUSH(TEXT("IconRestart64"), Icon20x20));
	// Style->Set("GamedevHelper.Cmd_AssetNamingManagerOpenAsset", new IMAGE_BRUSH(TEXT("IconSearch64"), Icon40x40));
	// Style->Set("GamedevHelper.Cmd_AssetNamingManagerOpenAsset.Small", new IMAGE_BRUSH(TEXT("IconSearch64"), Icon20x20));
	// Style->Set("GamedevHelper.Cmd_AssetNamingManagerRenameSelected", new IMAGE_BRUSH(TEXT("IconRename64"), Icon40x40));
	// Style->Set("GamedevHelper.Cmd_AssetNamingManagerRenameSelected.Small", new IMAGE_BRUSH(TEXT("IconRename64"), Icon20x20));
	Style->Set("GamedevHelper.Cmd_OpenRenderingManagerWindow", new IMAGE_BRUSH(TEXT("IconRenderer128"), Icon40x40));
	Style->Set("GamedevHelper.Cmd_OpenRenderingManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconRenderer128"), Icon20x20));
	// Style->Set("GamedevHelper.Cmd_RenderingManagerRemoveRenderedImages", new IMAGE_BRUSH(TEXT("IconRemoveImg128"), Icon40x40));
	// Style->Set("GamedevHelper.Cmd_RenderingManagerRemoveRenderedImages.Small", new IMAGE_BRUSH(TEXT("IconRemoveImg128"), Icon20x20));
	// Style->Set("GamedevHelper.Cmd_RenderingManagerRemoveRenderedVideo", new IMAGE_BRUSH(TEXT("IconRemoveVideo128"), Icon40x40));
	// Style->Set("GamedevHelper.Cmd_RenderingManagerRemoveRenderedVideo.Small", new IMAGE_BRUSH(TEXT("IconRemoveVideo128"), Icon20x20));
	
	// colors
	Style->Set("GamedevHelper.Color.Red", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#C62828"))}});
	Style->Set("GamedevHelper.Color.Green", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#2E7D32"))}});
	Style->Set("GamedevHelper.Color.Grey", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#616161"))}});

	return Style;
}
