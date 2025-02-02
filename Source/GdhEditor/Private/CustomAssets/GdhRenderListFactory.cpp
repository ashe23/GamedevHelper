// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/GdhRenderListFactory.h"
#include "CustomAssets/GdhRenderList.h"

UGdhRenderListFactory::UGdhRenderListFactory() {
	SupportedClass = UGdhRenderList::StaticClass();
	bCreateNew = true;
}

UObject* UGdhRenderListFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
) {
	return NewObject<UGdhRenderList>(InParent, InClass, InName, Flags, Context);
}
