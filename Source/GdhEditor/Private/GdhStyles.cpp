// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhStyles.h"
#include "GdhConstants.h"
// Engine Headers
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

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
	Style->Set("GamedevHelper.RestartEditor", new IMAGE_BRUSH(TEXT("IconRestart32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.RestartEditor.Small", new IMAGE_BRUSH(TEXT("IconRestart20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.OpenAssetNamingTool", new IMAGE_BRUSH(TEXT("IconAssetNamingTool32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.OpenAssetNamingTool.Small", new IMAGE_BRUSH(TEXT("IconAssetNamingTool20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.ScanAssets", new IMAGE_BRUSH(TEXT("IconRefresh32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.ScanAssets.Small", new IMAGE_BRUSH(TEXT("IconRefresh20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.RenameAssets", new IMAGE_BRUSH(TEXT("IconRename32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.RenameAssets.Small", new IMAGE_BRUSH(TEXT("IconRename20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.ClearSelection", new IMAGE_BRUSH(TEXT("IconClear32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.ClearSelection.Small", new IMAGE_BRUSH(TEXT("IconClear20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.EditMode", new IMAGE_BRUSH(TEXT("IconEditMode32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.EditMode.Small", new IMAGE_BRUSH(TEXT("IconEditMode20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.ApplyChanges", new IMAGE_BRUSH(TEXT("IconCheck32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.ApplyChanges.Small", new IMAGE_BRUSH(TEXT("IconCheck20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.UndoChanges", new IMAGE_BRUSH(TEXT("IconUndo32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.UndoChanges.Small", new IMAGE_BRUSH(TEXT("IconUndo20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.NumerizeAssets", new IMAGE_BRUSH(TEXT("IconNumerize32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.NumerizeAssets.Small", new IMAGE_BRUSH(TEXT("IconNumerize20"), FVector2D{20.0f, 20.0f}));
	Style->Set("GamedevHelper.SearchAndReplace", new IMAGE_BRUSH(TEXT("IconFind32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.SearchAndReplace.Small", new IMAGE_BRUSH(TEXT("IconFind20"), FVector2D{20.0f, 20.0f}));

	// tabs
	Style->Set("GamedevHelper.Tab.AssetNamingTool", new IMAGE_BRUSH(TEXT("IconAssetNamingTool16"), FVector2D{16.0f, 16.0f}));
	Style->Set("GamedevHelper.Tab.WorldOutlinearManager", new IMAGE_BRUSH(TEXT("IconStructure16"), FVector2D{16.0f, 16.0f}));

	// icons
	Style->Set("GamedevHelper.Icon.Arrow", new IMAGE_BRUSH(TEXT("IconArrow64"), FVector2D{16.0f, 16.0f}));
	Style->Set("GamedevHelper.Icon.Bg", new IMAGE_BRUSH(TEXT("IconBg16"), FVector2D{16.0f, 16.0f}));
	Style->Set("GamedevHelper.Icon.Check", new IMAGE_BRUSH(TEXT("IconCheck32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Icon.Warning", new IMAGE_BRUSH(TEXT("IconWarning32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Icon.Modified", new IMAGE_BRUSH(TEXT("IconModified32"), FVector2D{32.0f, 32.0f}));

	// colors
	Style->Set("GamedevHelper.Color.Red", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#d63232"))}});
	Style->Set("GamedevHelper.Color.Green", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#228B22"))}});
	Style->Set("GamedevHelper.Color.Grey", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#616161"))}});
	Style->Set("GamedevHelper.Color.GreyDark", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#484848"))}});
	Style->Set("GamedevHelper.Color.Title", FSlateColor{FLinearColor{FColor::FromHex(TEXT("#17C3B2"))}});

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
