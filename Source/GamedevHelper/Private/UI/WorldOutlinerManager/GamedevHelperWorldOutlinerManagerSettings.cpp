// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/WorldOutlinerManager/GamedevHelperWorldOutlinerManagerSettings.h"
// Engine Headers
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Engine/RectLight.h"
#include "Engine/SpotLight.h"

UGamedevHelperWorldOutlinerSettings::UGamedevHelperWorldOutlinerSettings()
{
	SetDefaultSettings();
}

FName UGamedevHelperWorldOutlinerSettings::GetFolderNameByActor(const AActor* Actor)
{
	if (!Actor)
	{
		return NAME_None;
	}

	const UClass* OtherActorClass = Actor->GetClass();
	if (!OtherActorClass)
	{
		return NAME_None;
	}

	for (const auto Mapping : ActorFolderMappings)
	{
		UClass* MappingClass = Mapping.Key;
		if (!MappingClass) continue;

		if (MappingClass == OtherActorClass || OtherActorClass->IsChildOf(MappingClass))
		{
			return Mapping.Value;
		}
	}

	return NAME_None;
}

void UGamedevHelperWorldOutlinerSettings::SetDefaultSettings()
{
	// todo:ashe23
	ActorFolderMappings.Add(APointLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ASpotLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ARectLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ADirectionalLight::StaticClass(), TEXT("Light"));
}
