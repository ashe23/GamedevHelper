// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhStyles.h"
#include "Gdh.h"
// Engine Headers
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

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

FSlateIcon FGdhStyles::GetIcon(const FString& IconSpecifier)
{
	return FSlateIcon(GetStyleSetName(), FName{*IconSpecifier});
}

const FSlateBrush* FGdhStyles::GetIconBrush(const FString& Specifier)
{
	return Get().GetBrush(*Specifier);
}

FSlateColor FGdhStyles::GetColor(const FString& Specifier)
{
	return Get().GetSlateColor(*Specifier);
}

FSlateFontInfo FGdhStyles::GetFont(const FString& FontType, const uint32 FontSize)
{
	return FCoreStyle::GetDefaultFontStyle(*FontType, FontSize);
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

TSharedRef<FSlateStyleSet> FGdhStyles::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GdhConstants::ModuleStylesName));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(GdhConstants::ModuleFullName.ToString())->GetBaseDir() / TEXT("Resources"));

	// cmds
	Style->Set("GamedevHelper.TabMain", new IMAGE_BRUSH(TEXT("Icon40"), FVector2D{40.0f, 40.0f}));
	Style->Set("GamedevHelper.Cmd_RestartEditor", new IMAGE_BRUSH(TEXT("IconRefresh32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_RestartEditor.Small", new IMAGE_BRUSH(TEXT("IconRefresh20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.Cmd_OpenAssetNamingManager", new IMAGE_BRUSH(TEXT("IconRename32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_OpenAssetNamingManager.Small", new IMAGE_BRUSH(TEXT("IconRename20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.Cmd_OpenWorldOutlinearManager", new IMAGE_BRUSH(TEXT("IconStructure32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_OpenWorldOutlinearManager.Small", new IMAGE_BRUSH(TEXT("IconStructure20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.Cmd_OpenMrqBatchRenderManager", new IMAGE_BRUSH(TEXT("IconRender32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_OpenMrqBatchRenderManager.Small", new IMAGE_BRUSH(TEXT("IconRender20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.Cmd_ClearSelection", new IMAGE_BRUSH(TEXT("IconClear32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_ClearSelection.Small", new IMAGE_BRUSH(TEXT("IconClear20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.Cmd_ScanAssets", new IMAGE_BRUSH(TEXT("IconRefresh32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_ScanAssets.Small", new IMAGE_BRUSH(TEXT("IconRefresh20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.Cmd_RenameAssets", new IMAGE_BRUSH(TEXT("IconRename32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_RenameAssets.Small", new IMAGE_BRUSH(TEXT("IconRename20"), FVector2D{20.0f, 20.0f}));

	// tabs
	Style->Set("GamedevHelper.Tab.AssetNamingManager", new IMAGE_BRUSH(TEXT("IconRename16"), FVector2D{16.0f, 16.0f}));

	// icons
	Style->Set("GamedevHelper.Icon.Arrow", new IMAGE_BRUSH(TEXT("IconArrow64"), FVector2D{16.0f, 16.0f}));
	Style->Set("GamedevHelper.Icon.Bg", new IMAGE_BRUSH(TEXT("IconBg16"), FVector2D{16.0f, 16.0f}));

	// colors
	Style->Set("GamedevHelper.Color.Red", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#C62828"))}});
	Style->Set("GamedevHelper.Color.Green", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#2E7D32"))}});
	Style->Set("GamedevHelper.Color.Grey", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#616161"))}});
	Style->Set("GamedevHelper.Color.Title", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#17C3B2"))}});


	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
