// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/GdhVideoPipelineFactory.h"
#include "CustomAssets/GdhVideoPipeline.h"

UGdhVideoPipelineFactory::UGdhVideoPipelineFactory() {
	SupportedClass = UGdhVideoPipeline::StaticClass();
	bCreateNew = true;
}

UObject* UGdhVideoPipelineFactory::FactoryCreateNew(
	UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn
) {
	return NewObject<UGdhVideoPipeline>(InParent, InClass, InName, Flags, Context);
}
