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

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

TSharedRef<FSlateStyleSet> FGdhStyles::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GdhConstants::ModuleStylesName));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(GdhConstants::ModuleFullName.ToString())->GetBaseDir() / TEXT("Resources"));

	Style->Set("GamedevHelper.TabMain", new IMAGE_BRUSH(TEXT("Icon40"), FVector2D{40.0f, 40.0f}));
	Style->Set("GamedevHelper.Cmd_RestartEditor", new IMAGE_BRUSH(TEXT("IconRefresh32"), FVector2D{32.0f, 32.0f}));
	Style->Set("GamedevHelper.Cmd_RestartEditor.Small", new IMAGE_BRUSH(TEXT("IconRefresh20"), FVector2D{20.0f, 20.0f}));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
