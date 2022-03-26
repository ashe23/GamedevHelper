// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperProjectSettings.h"
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
// Blueprint classes
#include "EditorUtilityWidgetBlueprint.h"
#include "Blutility/Classes/EditorUtilityBlueprint.h"
#include "WidgetBlueprint.h"
#include "GameFramework/GameModeBase.h"
#include "EditorTutorial.h"
// Material classes
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialFunction.h"
#include "Materials/MaterialParameterCollection.h"
#include "Engine/Classes/Materials/MaterialFunctionMaterialLayer.h"
#include "Engine/Classes/Materials/MaterialFunctionMaterialLayerBlend.h"
#include "Engine/SubsurfaceProfile.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
// Texture classes
#include "Engine/Texture2D.h"
#include "Engine/TextureCube.h"
#include "Engine/Texture2DArray.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/TextureRenderTarget2DArray.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Engine/TextureRenderTargetVolume.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/VolumeTexture.h"
#include "MediaTexture.h"
#include "VT/RuntimeVirtualTexture.h"
// Animation classes
#include "Animation/AnimSequence.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimComposite.h"
#include "Animation/BlendSpace.h"
#include "Animation/BlendSpace1D.h"
#include "Animation/Skeleton.h"
#include "Animation/PoseAsset.h"
#include "Animation/AnimLayerInterface.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Animation/AimOffsetBlendSpace1D.h"
#include "Animation/AnimBoneCompressionSettings.h"
#include "Animation/AnimCurveCompressionSettings.h"
#include "Engine/SkeletalMesh.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "LevelSequence.h"
#include "Camera/CameraAnim.h"
// FX
#include "Particles/ParticleSystem.h"
#include "NiagaraEffectType.h"
#include "NiagaraScript.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"
#include "NiagaraParameterCollection.h"
#include "NiagaraParameterDefinitions.h"
// AI
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "EnvironmentQuery/EnvQuery.h"
// Foliage
#include "FoliageType_Actor.h"
#include "FoliageType_InstancedStaticMesh.h"
#include "LandscapeGrassType.h"
// Slate
#include "Slate/SlateBrushAsset.h"
#include "Slate/SlateVectorArtData.h"
#include "Styling/SlateWidgetStyleAsset.h"
#include "Engine/Font.h"
// Paper2D
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
// Sound
#include "Sound/DialogueVoice.h"
#include "Sound/DialogueWave.h"
#include "Sound/ReverbEffect.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundConcurrency.h"
// Misc
#include "GamedevHelperAssetLibrary.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveVector.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/UserDefinedStruct.h"


UGamedevHelperAssetNamingConventionSettings::UGamedevHelperAssetNamingConventionSettings()
{
	// blueprint type prefixes
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::Normal,TEXT("BP"));
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::Interface,TEXT("BPI"));
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::FunctionLibrary,TEXT("BPFL"));
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::MacroLibrary,TEXT("BPML"));

	// blueprints
	Mappings.Add(UBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP")});
	Mappings.Add(AActor::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP")});
	Mappings.Add(UActorComponent::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Comp")});
	Mappings.Add(APawn::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Pawn")});
	Mappings.Add(ACharacter::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Char")});
	Mappings.Add(APlayerController::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Ctrl")});
	Mappings.Add(AGameModeBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("GM")});
	Mappings.Add(UWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BPW")});
	Mappings.Add(UEditorTutorial::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Tut")});
	Mappings.Add(UEditorUtilityBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("EUB")});
	Mappings.Add(UEditorUtilityWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("EUW")});

	// animations
	Mappings.Add(UAnimSequence::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AN")});
	Mappings.Add(UAnimMontage::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AM")});
	Mappings.Add(UAnimComposite::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AC")});
	Mappings.Add(UBlendSpace::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ABS")});
	Mappings.Add(UBlendSpace1D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ABS")});
	Mappings.Add(UAimOffsetBlendSpace::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AO")});
	Mappings.Add(UAimOffsetBlendSpace1D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AO")});
	Mappings.Add(USkeletalMesh::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SKM")});
	Mappings.Add(UStaticMesh::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SM")});
	Mappings.Add(USkeleton::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SK")});
	Mappings.Add(UPoseAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("A"), TEXT("Pose")});
	Mappings.Add(UPhysicsAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("PA")});
	Mappings.Add(UAnimBoneCompressionSettings::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("A"), TEXT("BCS")});
	Mappings.Add(UAnimCurveCompressionSettings::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("A"), TEXT("CCS")});
	Mappings.Add(UAnimBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ABP")});
	Mappings.Add(UAnimLayerInterface::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ALI")});
	Mappings.Add(ULevelSequence::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("LS")});
	Mappings.Add(UCameraAnim::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("LS")});

	// ai
	Mappings.Add(UBehaviorTree::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BT")});
	Mappings.Add(UBlackboardData::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BB")});
	Mappings.Add(UEnvQuery::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("EQS")});
	Mappings.Add(UBTTask_BlueprintBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AI"), TEXT("Task")});
	Mappings.Add(UBTService_BlueprintBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AI"), TEXT("Service")});
	Mappings.Add(UBTDecorator_BlueprintBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AI"), TEXT("Decorator")});

	// materials
	Mappings.Add(UMaterial::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("M")});
	Mappings.Add(UMaterialInstance::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MI")});
	Mappings.Add(UMaterialInstanceConstant::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MI")});
	Mappings.Add(UMaterialInstanceDynamic::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MI")});
	Mappings.Add(UMaterialFunction::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MF")});
	Mappings.Add(UMaterialParameterCollection::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MF")});
	Mappings.Add(USubsurfaceProfile::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SSP")});
	Mappings.Add(UPhysicalMaterial::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("PM")});
	Mappings.Add(UMaterialFunctionMaterialLayer::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ML")});
	Mappings.Add(UMaterialFunctionMaterialLayerBlend::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MLB")});

	// textures
	Mappings.Add(UTexture2D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T")});
	Mappings.Add(UTextureCube::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("HDR")});
	Mappings.Add(UMediaTexture::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("Media")});
	Mappings.Add(UVolumeTexture::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("Volume")});
	Mappings.Add(URuntimeVirtualTexture::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("RVT")});
	Mappings.Add(UTexture2DArray::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("2DA")});

	// Render Targets
	Mappings.Add(UTextureRenderTarget2D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Mappings.Add(UTextureRenderTarget2DArray::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Mappings.Add(UCanvasRenderTarget2D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Mappings.Add(UTextureRenderTargetCube::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Mappings.Add(UTextureRenderTargetVolume::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});

	// fx
	Mappings.Add(UParticleSystem::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX")});
	Mappings.Add(UNiagaraScript::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("Script")});
	Mappings.Add(UNiagaraEffectType::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"),TEXT("Type")});
	Mappings.Add(UNiagaraEmitter::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("Emitter")});
	Mappings.Add(UNiagaraSystem::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("System")});
	Mappings.Add(UNiagaraParameterCollection::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("PC")});
	Mappings.Add(UNiagaraParameterCollectionInstance::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("PCI")});
	Mappings.Add(UNiagaraParameterDefinitions::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("Def")});

	// foliage
	Mappings.Add(UFoliageType_Actor::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FOL")});
	Mappings.Add(UFoliageType_InstancedStaticMesh::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FOL"), TEXT("ISM")});
	Mappings.Add(ULandscapeGrassType::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FOL"), TEXT("LGT")});

	// paper2d
	Mappings.Add(UPaperFlipbook::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("Flipbook")});
	Mappings.Add(UPaperSprite::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("Sprite")});
	Mappings.Add(UPaperTileMap::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("TileMap")});
	Mappings.Add(UPaperTileSet::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("TileSet")});

	// slate
	Mappings.Add(USlateBrushAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SLT"), TEXT("Brush")});
	Mappings.Add(USlateWidgetStyleAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SLT"), TEXT("WSA")});
	Mappings.Add(UFont::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("Font")});
	Mappings.Add(USlateVectorArtData::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SLT"), TEXT("VA")});

	// sound
	Mappings.Add(UDialogueVoice::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("DV")});
	Mappings.Add(UDialogueWave::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("DW")});
	Mappings.Add(USoundClass::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Class")});
	Mappings.Add(USoundConcurrency::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Concurrency")});
	Mappings.Add(USoundCue::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Cue")});
	Mappings.Add(USoundMix::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Mix")});
	Mappings.Add(USoundWave::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Wave")});
	Mappings.Add(UReverbEffect::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SX")});
	Mappings.Add(USoundAttenuation::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SX")});
	// todo:ashe23 update list for sounds

	// misc
	Mappings.Add(UDataAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("DA")});
	Mappings.Add(UDataTable::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("DT")});
	Mappings.Add(UCurveFloat::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("C"), TEXT("F")});
	Mappings.Add(UCurveVector::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("C"), TEXT("V")});
	Mappings.Add(UCurveLinearColor::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("C"), TEXT("LC")});
	Mappings.Add(UEnum::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("E")});
	Mappings.Add(UUserDefinedEnum::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("E")});
	Mappings.Add(UUserDefinedStruct::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("F")});
}

FGamedevHelperAssetNameFormat UGamedevHelperAssetNamingConventionSettings::GetNameFormatByAssetData(const FAssetData& Asset) const
{
	if (!Asset.IsValid())
	{
		return FGamedevHelperAssetNameFormat{};
	}

	if (!Asset.AssetClass.IsEqual(TEXT("Blueprint")))
	{
		return GetNameFormatByClass(Asset.GetClass());
	}

	const UClass* ParentClass = UGamedevHelperAssetLibrary::GetBlueprintParentClass(Asset);
	const UClass* SearchClass = ParentClass ? ParentClass : UBlueprint::StaticClass();
	FGamedevHelperAssetNameFormat NameFormat = GetNameFormatByClass(SearchClass);

	if (NameFormat.Prefix.IsEmpty())
	{
		const auto BlueprintType = UGamedevHelperAssetLibrary::GetBlueprintType(Asset);
		const auto BlueprintTypePrefix = BlueprintTypePrefixes.Find(BlueprintType);
		const FString BlueprintPrefix = BlueprintTypePrefix ? *BlueprintTypePrefix : TEXT("BP");
		NameFormat.Prefix = BlueprintPrefix;
	}

	return NameFormat;
}

FGamedevHelperAssetNameFormat UGamedevHelperAssetNamingConventionSettings::GetNameFormatByClass(const UClass* SearchClass) const
{
	if (!SearchClass)
	{
		return FGamedevHelperAssetNameFormat{};
	}

	for (const auto& Naming : Mappings)
	{
		const UClass* NamingClass = Naming.Key;
		if (NamingClass == SearchClass)
		{
			return Naming.Value;
		}
	}

	return FGamedevHelperAssetNameFormat{};
}

UGamedevHelperWorldOutlinerSettings::UGamedevHelperWorldOutlinerSettings()
{
	Mappings.Add(APointLight::StaticClass(), TEXT("Light"));
	Mappings.Add(ASpotLight::StaticClass(), TEXT("Light"));
	Mappings.Add(ARectLight::StaticClass(), TEXT("Light"));
	Mappings.Add(ADirectionalLight::StaticClass(), TEXT("Light"));
	Mappings.Add(ASkyLight::StaticClass(), TEXT("Light"));
	Mappings.Add(ASkyAtmosphere::StaticClass(), TEXT("Light"));
	Mappings.Add(AVolumetricCloud::StaticClass(), TEXT("Light"));
	Mappings.Add(AExponentialHeightFog::StaticClass(), TEXT("Light"));
	Mappings.Add(AStaticMeshActor::StaticClass(), TEXT("Geometry"));
	Mappings.Add(ABrush::StaticClass(), TEXT("Geometry"));
	Mappings.Add(APlayerStart::StaticClass(), TEXT("Gameplay"));
	Mappings.Add(APawn::StaticClass(), TEXT("Gameplay"));
	Mappings.Add(ACharacter::StaticClass(), TEXT("Gameplay"));
	Mappings.Add(ATriggerBox::StaticClass(), TEXT("Triggers"));
	Mappings.Add(ATriggerSphere::StaticClass(), TEXT("Triggers"));
	Mappings.Add(ATriggerVolume::StaticClass(), TEXT("Triggers"));
	Mappings.Add(ACameraRig_Crane::StaticClass(), TEXT("Cinematics"));
	Mappings.Add(ACameraRig_Rail::StaticClass(), TEXT("Cinematics"));
	Mappings.Add(ACineCameraActor::StaticClass(), TEXT("Cinematics"));
	Mappings.Add(ALevelSequenceActor::StaticClass(), TEXT("Cinematics"));
	Mappings.Add(APostProcessVolume::StaticClass(), TEXT("PostProcess"));
	Mappings.Add(ASphereReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	Mappings.Add(ABoxReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	Mappings.Add(APlaneReflectionCapture::StaticClass(), TEXT("ReflectionCaptures"));
	Mappings.Add(ADecalActor::StaticClass(), TEXT("Decals"));
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

	for (const auto Mapping : Mappings)
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
