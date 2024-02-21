// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/GdhAssetNamingToolSettings.h"
#include "GdhStructs.h"
// Engine Headers
#include "UObject/ConstructorHelpers.h"

UGdhAssetNamingToolSettings::UGdhAssetNamingToolSettings()
{
	// BlueprintTypes.Add(EGdhBlueprintType::Normal, FGdhAffix{TEXT("BP")});
	// BlueprintTypes.Add(EGdhBlueprintType::Interface, FGdhAffix{TEXT("BPI")});
	// BlueprintTypes.Add(EGdhBlueprintType::FunctionLibrary, FGdhAffix{TEXT("BPFL")});
	// BlueprintTypes.Add(EGdhBlueprintType::MacroLibrary, FGdhAffix{TEXT("BPML")});
	//
	// static ConstructorHelpers::FObjectFinder<UDataTable> DataTable{TEXT("DataTable'/GamedevHelper/NamingConvention/DT_Affix_Mappings_Unreal.DT_Affix_Mappings_Unreal'")};
	// if (DataTable.Succeeded())
	// {
	// 	Mappings = DataTable.Object;
	// }
}

#if WITH_EDITOR
void UGdhAssetNamingToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig();
}
#endif
