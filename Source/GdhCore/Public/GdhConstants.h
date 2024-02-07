// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace GdhConstants
{
	// modules
	static const FName ModuleFullName{TEXT("GamedevHelper")};
	static const FName ModuleName{TEXT("Gdh")}; // Gdh - GamedevHelper
	static const FName ModuleStylesName{TEXT("GamedevHelperStyle")};
	static const FName ModuleAssetRegistry{TEXT("AssetRegistry")};
	static const FName ModulePropertyEditor{TEXT("PropertyEditor")};
	static const FName ModuleAssetTools{TEXT("AssetTools")};
	static const FName ModuleContentBrowser{TEXT("ContentBrowser")};

	// paths
	static const FName PathRoot{TEXT("/Game")};

	// tabs
	static const FName TabAssetNamingTool{TEXT("TabAssetNamingTool")};

	// charsets
	static const FString AlphaLower = TEXT("abcdefghijklmnopqrstuvwxyz");
	static const FString AlphaUpper = AlphaLower.ToUpper();
	static const FString AlphaMixed = AlphaLower + AlphaLower.ToUpper();
	static const FString Digits = TEXT("0123456789");
}
