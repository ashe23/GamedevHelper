// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetRenderListFactory.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"

UGdhBetRenderListFactory::UGdhBetRenderListFactory() {
	SupportedClass = UGdhBetRenderList::StaticClass();
	bCreateNew = true;
}

UObject* UGdhBetRenderListFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
) {
	return NewObject<UGdhBetRenderList>(InParent, InClass, InName, Flags, Context);
}
