// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/WorldOutlinerManager/GamedevHelperWorldOutlinerManagerSettings.h"
// Engine Headers
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Engine/RectLight.h"
#include "Engine/SpotLight.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TriggerBox.h"
#include "Engine/TriggerSphere.h"
#include "Engine/TriggerVolume.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SphereReflectionCapture.h"
#include "Engine/BoxReflectionCapture.h"
#include "Engine/PlaneReflectionCapture.h"
#include "Engine/DecalActor.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "CameraRig_Crane.h"
#include "CameraRig_Rail.h"
#include "CineCameraActor.h"
#include "LevelSequenceActor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"

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
	ActorFolderMappings.Add(APointLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ASpotLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ARectLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ADirectionalLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ASkyLight::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(ASkyAtmosphere::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(AVolumetricCloud::StaticClass(), TEXT("Light"));
	ActorFolderMappings.Add(AExponentialHeightFog::StaticClass(), TEXT("Light"));
	
	ActorFolderMappings.Add(AStaticMeshActor::StaticClass(), TEXT("Geometry"));
	
	ActorFolderMappings.Add(APlayerStart::StaticClass(), TEXT("Gameplay"));
	ActorFolderMappings.Add(APawn::StaticClass(), TEXT("Gameplay"));
	ActorFolderMappings.Add(ACharacter::StaticClass(), TEXT("Gameplay"));
	
	ActorFolderMappings.Add(ATriggerBox::StaticClass(), TEXT("Triggers"));
	ActorFolderMappings.Add(ATriggerSphere::StaticClass(), TEXT("Triggers"));
	ActorFolderMappings.Add(ATriggerVolume::StaticClass(), TEXT("Triggers"));
	
	ActorFolderMappings.Add(ACameraRig_Crane::StaticClass(), TEXT("Cinematics"));
	ActorFolderMappings.Add(ACameraRig_Rail::StaticClass(), TEXT("Cinematics"));
	ActorFolderMappings.Add(ACineCameraActor::StaticClass(), TEXT("Cinematics"));
	ActorFolderMappings.Add(ALevelSequenceActor::StaticClass(), TEXT("Cinematics"));
	
	ActorFolderMappings.Add(APostProcessVolume::StaticClass(), TEXT("PostProcess"));
	
	ActorFolderMappings.Add(ASphereReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	ActorFolderMappings.Add(ABoxReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	ActorFolderMappings.Add(APlaneReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	
	ActorFolderMappings.Add(ADecalActor::StaticClass(), TEXT("Decals"));
}
