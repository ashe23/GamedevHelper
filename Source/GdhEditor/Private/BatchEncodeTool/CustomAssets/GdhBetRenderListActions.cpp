// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetRenderListActions.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderListEdToolkit.h"

FGdhBetRenderListActions::FGdhBetRenderListActions(
	const EAssetTypeCategories::Type& InAssetTypeCategory
) :
	Category(InAssetTypeCategory) {}

UClass* FGdhBetRenderListActions::GetSupportedClass() const {
	return UGdhBetRenderList::StaticClass();
}

FText FGdhBetRenderListActions::GetName() const {
	return FText::FromName("Gdh Render List");
}

FColor FGdhBetRenderListActions::GetTypeColor() const {
	return FColor::Black;
}

uint32 FGdhBetRenderListActions::GetCategories() {
	return Category;
}

void FGdhBetRenderListActions::OpenAssetEditor(
	const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor
) {
	// const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	//
	// for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	// {
	// 	if (UPaperSprite* Sprite = Cast<UPaperSprite>(*ObjIt))
	// 	{
	// 		TSharedRef<FSpriteEditor> NewSpriteEditor(new FSpriteEditor());
	// 		NewSpriteEditor->InitSpriteEditor(Mode, EditWithinLevelEditor, Sprite);
	// 	}
	// }
	MakeShared<FGdhBetRenderListEdToolkit>()->InitEditor(InObjects);
}
