// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhPluginSettings.h"
#include "GdhConstants.h"
#include "GdhEnums.h"
#include "GdhStructs.h"
// Engine Headers
#include "UObject/ConstructorHelpers.h"

UGdhPluginSettings::UGdhPluginSettings()
{
	BlueprintTypeAffixes.Add(EGdhBlueprintType::Normal, FGdhAffix{TEXT("BP")});
	BlueprintTypeAffixes.Add(EGdhBlueprintType::Interface, FGdhAffix{TEXT("BPI")});
	BlueprintTypeAffixes.Add(EGdhBlueprintType::FunctionLibrary, FGdhAffix{TEXT("BPFL")});
	BlueprintTypeAffixes.Add(EGdhBlueprintType::MacroLibrary, FGdhAffix{TEXT("BPML")});

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable{TEXT("DataTable'/GamedevHelper/NamingConvention/DT_Asset_Affixes_Unreal.DT_Asset_Affixes_Unreal'")};
	if (DataTable.Succeeded())
	{
		AssetAffixes = DataTable.Object;
	}
}

FName UGdhPluginSettings::GetContainerName() const
{
	return FName{TEXT("Project")};
}

FName UGdhPluginSettings::GetCategoryName() const
{
	return FName{TEXT("Plugins")};
}

FName UGdhPluginSettings::GetSectionName() const
{
	return GdhConstants::ModuleFullName;
}

#if WITH_EDITOR
FText UGdhPluginSettings::GetSectionText() const
{
	return FText::FromName(GdhConstants::ModuleFullName);
}

void UGdhPluginSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
