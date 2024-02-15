// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/GdhAssetNamingToolSettings.h"
#include "GdhStructs.h"

UGdhAssetNamingToolSettings::UGdhAssetNamingToolSettings()
{
	BlueprintTypes.Add(EGdhBlueprintType::Normal, FGdhAssetNameAffix{TEXT("BP")});
	BlueprintTypes.Add(EGdhBlueprintType::Interface, FGdhAssetNameAffix{TEXT("BPI")});
	BlueprintTypes.Add(EGdhBlueprintType::FunctionLibrary, FGdhAssetNameAffix{TEXT("BPFL")});
	BlueprintTypes.Add(EGdhBlueprintType::MacroLibrary, FGdhAssetNameAffix{TEXT("BPML")});
}

#if WITH_EDITOR
void UGdhAssetNamingToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig();
}
#endif
