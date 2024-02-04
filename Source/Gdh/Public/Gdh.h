// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdh, Log, All);

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
	// static const FName TabAssetNamingManager{TEXT("TabGdhAssetNamingManager")};
	static const FName TabWorldOutlinearManager{TEXT("TabGdhWorldOutlinearManager")};

	// misc
	static const TSet<FString> SourceFileExtensions{TEXT("cpp"), TEXT("h"), TEXT("cs")};
	static const TSet<FString> ConfigFileExtensions{TEXT("ini")};
	static constexpr int32 WidgetIndexIdle = 0;
	static constexpr int32 WidgetIndexWorking = 1;

	template <typename EnumType>
	static EnumType GetEnumValueFromString(const FString& String)
	{
		return static_cast<EnumType>(StaticEnum<EnumType>()->GetValueByName(FName(*String)));
	}
}

class FGdh final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;

private:
	TSharedPtr<FUICommandList> Commands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};
