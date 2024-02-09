// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibEditor.generated.h"

class FContentBrowserModule;
class FAssetRegistryModule;
class FAssetToolsModule;

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhEditorLibrary"))
class GDHUTIL_API UGdhLibEditor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FAssetToolsModule& GetModuleAssetTools();
	static FAssetRegistryModule& GetModuleAssetRegistry();
	static FContentBrowserModule& GetModuleContentBrowser();
	static FPropertyEditorModule& GetModulePropertyEditor();
};
