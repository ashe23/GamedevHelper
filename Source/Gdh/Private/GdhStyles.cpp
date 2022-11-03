// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhStyles.h"
#include "Gdh.h"
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
const FVector2D Icon32x32(32.0f, 32.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedPtr<FSlateStyleSet> FGdhStyles::StyleInstance = nullptr;

void FGdhStyles::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGdhStyles::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

void FGdhStyles::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FGdhStyles::Get()
{
	return *StyleInstance;
}

FName FGdhStyles::GetStyleSetName()
{
	return GdhConstants::ModuleStylesName;
}

const FSlateBrush* FGdhStyles::GetIcon(const FString& Specifier)
{
	return Get().GetBrush(*Specifier);
}

FSlateColor FGdhStyles::GetColor(const FString& Specifier)
{
	return Get().GetSlateColor(*Specifier);
}

FString FGdhStyles::GetIconByStatus(const EGdhGenericStatus Status)
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

FLinearColor FGdhStyles::GetColorByStatus(const EGdhGenericStatus Status)
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

TSharedRef<FSlateStyleSet> FGdhStyles::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GdhConstants::ModuleStylesName));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(GdhConstants::ModuleFullName.ToString())->GetBaseDir() / TEXT("Resources"));

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
	Style->Set("GamedevHelper.Icon.Check32", new IMAGE_BRUSH(TEXT("IconCheck128"), Icon32x32));
	Style->Set("GamedevHelper.Icon.Check40", new IMAGE_BRUSH(TEXT("IconCheck128"), Icon40x40));
	Style->Set("GamedevHelper.Icon.Cross20", new IMAGE_BRUSH(TEXT("IconCross128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.Cross32", new IMAGE_BRUSH(TEXT("IconCross128"), Icon32x32));
	Style->Set("GamedevHelper.Icon.Cross40", new IMAGE_BRUSH(TEXT("IconCross128"), Icon40x40));
	Style->Set("GamedevHelper.Icon.Warning20", new IMAGE_BRUSH(TEXT("IconWarning128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.Warning32", new IMAGE_BRUSH(TEXT("IconWarning128"), Icon32x32));
	Style->Set("GamedevHelper.Icon.Warning40", new IMAGE_BRUSH(TEXT("IconWarning128"), Icon40x40));
	Style->Set("GamedevHelper.Icon.Refresh", new IMAGE_BRUSH(TEXT("IconRefresh128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.Render", new IMAGE_BRUSH(TEXT("IconRender128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.Bin", new IMAGE_BRUSH(TEXT("IconBin128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.OpenFolder", new IMAGE_BRUSH(TEXT("IconOpenFolder128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.RemoveFolder", new IMAGE_BRUSH(TEXT("IconRemoveFolder128"), Icon20x20));
	Style->Set("GamedevHelper.Icon.VertexAnim", new IMAGE_BRUSH(TEXT("IconVertex128"), Icon16x16));
	Style->Set("GamedevHelper.Icon.Collision", new IMAGE_BRUSH(TEXT("IconCollision128"), Icon16x16));
	// Style->Set("Gdh.Cmd_AssetNamingManagerWindow", new IMAGE_BRUSH(TEXT("IconRename64"), Icon40x40));
	// Style->Set("Gdh.Cmd_AssetNamingManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconRename64"), Icon20x20));
	// Style->Set("Gdh.Cmd_WorldOutlinerManagerWindow", new IMAGE_BRUSH(TEXT("IconOrganize64"), Icon40x40));
	// Style->Set("Gdh.Cmd_WorldOutlinerManagerWindow.Small", new IMAGE_BRUSH(TEXT("IconOrganize64"), Icon20x20));
	Style->Set("GamedevHelper.Cmd_RestartEditor", new IMAGE_BRUSH(TEXT("IconRestart128"), Icon40x40));
	Style->Set("GamedevHelper.Cmd_RestartEditor.Small", new IMAGE_BRUSH(TEXT("IconRestart128"), Icon20x20));
	// Style->Set("Gdh.Cmd_AssetNamingManagerOpenAsset", new IMAGE_BRUSH(TEXT("IconSearch64"), Icon40x40));
	// Style->Set("Gdh.Cmd_AssetNamingManagerOpenAsset.Small", new IMAGE_BRUSH(TEXT("IconSearch64"), Icon20x20));
	// Style->Set("Gdh.Cmd_AssetNamingManagerRenameSelected", new IMAGE_BRUSH(TEXT("IconRename64"), Icon40x40));
	// Style->Set("Gdh.Cmd_AssetNamingManagerRenameSelected.Small", new IMAGE_BRUSH(TEXT("IconRename64"), Icon20x20));
	Style->Set("GamedevHelper.Cmd_OpenWindowRenderingManager", new IMAGE_BRUSH(TEXT("IconRenderer128"), Icon40x40));
	Style->Set("GamedevHelper.Cmd_OpenWindowRenderingManager.Small", new IMAGE_BRUSH(TEXT("IconRenderer128"), Icon20x20));
	// Style->Set("Gdh.Cmd_RenderingManagerRemoveRenderedImages", new IMAGE_BRUSH(TEXT("IconRemoveImg128"), Icon40x40));
	// Style->Set("Gdh.Cmd_RenderingManagerRemoveRenderedImages.Small", new IMAGE_BRUSH(TEXT("IconRemoveImg128"), Icon20x20));
	// Style->Set("Gdh.Cmd_RenderingManagerRemoveRenderedVideo", new IMAGE_BRUSH(TEXT("IconRemoveVideo128"), Icon40x40));
	// Style->Set("Gdh.Cmd_RenderingManagerRemoveRenderedVideo.Small", new IMAGE_BRUSH(TEXT("IconRemoveVideo128"), Icon20x20));

	// colors
	Style->Set("GamedevHelper.Color.Red", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#C62828"))}});
	Style->Set("GamedevHelper.Color.Green", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#2E7D32"))}});
	Style->Set("GamedevHelper.Color.Grey", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#616161"))}});

	return Style;
}
