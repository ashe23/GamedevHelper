// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhWorldOutlinearSettings.h"
#include "Gdh.h"
// Engine Headers
#include "Animation/SkeletalMeshActor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Engine/RectLight.h"
#include "Engine/SkyLight.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"


UGdhWorldOutlinearSettings::UGdhWorldOutlinearSettings()
{
	// Actors
	Mappings.Add(AActor::StaticClass(), TEXT("Actors"));
	Mappings.Add(AStaticMeshActor::StaticClass(), TEXT("Actors/Static"));
	Mappings.Add(ASkeletalMeshActor::StaticClass(), TEXT("Actors/Skeletal"));
	Mappings.Add(ACharacter::StaticClass(), TEXT("Actors/Characters"));
	Mappings.Add(APawn::StaticClass(), TEXT("Actors/Pawns"));
	
	// Lights and atmosphere
	Mappings.Add(ADirectionalLight::StaticClass(), TEXT("Lights"));
	Mappings.Add(APointLight::StaticClass(), TEXT("Lights"));
	Mappings.Add(ASpotLight::StaticClass(), TEXT("Lights"));
	Mappings.Add(ARectLight::StaticClass(), TEXT("Lights"));
	Mappings.Add(ASkyLight::StaticClass(), TEXT("Lights"));
	Mappings.Add(ASkyAtmosphere::StaticClass(), TEXT("Lights"));

	
	// Mappings.Add(AVolumetricCloud::StaticClass(), TEXT("Light"));
	// Mappings.Add(AExponentialHeightFog::StaticClass(), TEXT("Light"));
	// Mappings.Add(AStaticMeshActor::StaticClass(), TEXT("Geometry"));
	// Mappings.Add(ABrush::StaticClass(), TEXT("Geometry"));
	// Mappings.Add(APlayerStart::StaticClass(), TEXT("Gameplay"));
	// Mappings.Add(APawn::StaticClass(), TEXT("Gameplay"));
	// Mappings.Add(ACharacter::StaticClass(), TEXT("Gameplay"));
	// Mappings.Add(ATriggerBox::StaticClass(), TEXT("Triggers"));
	// Mappings.Add(ATriggerSphere::StaticClass(), TEXT("Triggers"));
	// Mappings.Add(ATriggerVolume::StaticClass(), TEXT("Triggers"));
	// Mappings.Add(ACameraRig_Crane::StaticClass(), TEXT("Cinematics"));
	// Mappings.Add(ACameraRig_Rail::StaticClass(), TEXT("Cinematics"));
	// Mappings.Add(ACineCameraActor::StaticClass(), TEXT("Cinematics"));
	// Mappings.Add(ALevelSequenceActor::StaticClass(), TEXT("Cinematics"));
	// Mappings.Add(APostProcessVolume::StaticClass(), TEXT("PostProcess"));
	// Mappings.Add(ASphereReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	// Mappings.Add(ABoxReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	// Mappings.Add(APlaneReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	// Mappings.Add(ADecalActor::StaticClass(), TEXT("Decals"));
}

FName UGdhWorldOutlinearSettings::GetContainerName() const
{
	return FName{"Project"};
}

FName UGdhWorldOutlinearSettings::GetCategoryName() const
{
	return GdhConstants::ModuleFullName;
}

FName UGdhWorldOutlinearSettings::GetSectionName() const
{
	return FName{"World Outlinear Settings"};
}

FString UGdhWorldOutlinearSettings::GetDesc()
{
	return TEXT("GamedevHelper World Outlinear Settings");
}

void UGdhWorldOutlinearSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();

	if (OnChangeDelegate.IsBound())
	{
		OnChangeDelegate.Execute();
	}
}

FGdhWorldOutlinearSettingsOnChangeDelegate& UGdhWorldOutlinearSettings::OnChange()
{
	return OnChangeDelegate;
}
