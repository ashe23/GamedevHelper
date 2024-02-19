// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/GdhAssetNamingToolSettings.h"
#include "GdhStructs.h"
// Engine Headers
#include "UObject/ConstructorHelpers.h"

UGdhAssetNamingToolSettings::UGdhAssetNamingToolSettings()
{
	BlueprintTypes.Add(EGdhBlueprintType::Normal, FGdhAssetNameAffix{TEXT("BP")});
	BlueprintTypes.Add(EGdhBlueprintType::Interface, FGdhAssetNameAffix{TEXT("BPI")});
	BlueprintTypes.Add(EGdhBlueprintType::FunctionLibrary, FGdhAssetNameAffix{TEXT("BPFL")});
	BlueprintTypes.Add(EGdhBlueprintType::MacroLibrary, FGdhAssetNameAffix{TEXT("BPML")});

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable{TEXT("DataTable'/GamedevHelper/NamingConvention/DT_Affix_Mappings_Basic.DT_Affix_Mappings_Basic'")};
	if (DataTable.Succeeded())
	{
		Mappings = DataTable.Object;
	}
}

#if WITH_EDITOR
void UGdhAssetNamingToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig();
}
#endif
