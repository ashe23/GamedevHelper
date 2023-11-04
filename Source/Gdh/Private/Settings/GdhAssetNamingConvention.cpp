// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhAssetNamingConvention.h"
// Engine Headers
// #include "Engine/DirectionalLight.h"
// #include "Engine/PointLight.h"
// #include "Engine/RectLight.h"
// #include "Engine/SpotLight.h"
// #include "Engine/SkyLight.h"
// #include "Engine/StaticMeshActor.h"
// #include "Engine/TriggerBox.h"
// #include "Engine/TriggerSphere.h"
// #include "Engine/TriggerVolume.h"
// #include "Engine/PostProcessVolume.h"
// #include "Engine/ExponentialHeightFog.h"
// #include "Engine/SphereReflectionCapture.h"
// #include "Engine/BoxReflectionCapture.h"
// #include "Engine/PlaneReflectionCapture.h"
// #include "Engine/DecalActor.h"
// #include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
// #include "CameraRig_Crane.h"
// #include "CameraRig_Rail.h"
// #include "CineCameraActor.h"
// #include "LevelSequenceActor.h"
// #include "Components/SkyAtmosphereComponent.h"
// #include "Components/VolumetricCloudComponent.h"
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
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveVector.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/UserDefinedStruct.h"

UGdhAssetNamingConvention::UGdhAssetNamingConvention()
{
	BlueprintPrefixes.Add(EGdhBlueprintType::Normal, FGdhAssetNamingInfo{TEXT("BP")});
	BlueprintPrefixes.Add(EGdhBlueprintType::Interface, FGdhAssetNamingInfo{TEXT("BPI")});
	BlueprintPrefixes.Add(EGdhBlueprintType::FunctionLibrary, FGdhAssetNamingInfo{TEXT("BPFL")});
	BlueprintPrefixes.Add(EGdhBlueprintType::MacroLibrary, FGdhAssetNamingInfo{TEXT("BPML")});

	// Level/Maps
	Mappings.Add(UWorld::StaticClass(), FGdhAssetNamingInfo{TEXT("LVL")});

	// blueprints
	Mappings.Add(UBlueprint::StaticClass(), FGdhAssetNamingInfo{TEXT("BP")});
	Mappings.Add(AActor::StaticClass(), FGdhAssetNamingInfo{TEXT("BP")});
	Mappings.Add(UActorComponent::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("Comp")});
	Mappings.Add(APawn::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("Pawn")});
	Mappings.Add(ACharacter::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("Char")});
	Mappings.Add(APlayerController::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("Ctrl")});
	Mappings.Add(AGameModeBase::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("GM")});
	Mappings.Add(UWidgetBlueprint::StaticClass(), FGdhAssetNamingInfo{TEXT("BPW")});
	Mappings.Add(UEditorTutorial::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("Tut")});
	Mappings.Add(UEditorUtilityBlueprint::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("EUB")});
	Mappings.Add(UEditorUtilityWidgetBlueprint::StaticClass(), FGdhAssetNamingInfo{TEXT("BP"), TEXT("EUW")});

	// animations
	Mappings.Add(UAnimSequence::StaticClass(), FGdhAssetNamingInfo{TEXT("AN")});
	Mappings.Add(UAnimMontage::StaticClass(), FGdhAssetNamingInfo{TEXT("AM")});
	Mappings.Add(UAnimComposite::StaticClass(), FGdhAssetNamingInfo{TEXT("AC")});
	Mappings.Add(UBlendSpace::StaticClass(), FGdhAssetNamingInfo{TEXT("ABS")});
	Mappings.Add(UBlendSpace1D::StaticClass(), FGdhAssetNamingInfo{TEXT("ABS")});
	Mappings.Add(UAimOffsetBlendSpace::StaticClass(), FGdhAssetNamingInfo{TEXT("AO")});
	Mappings.Add(UAimOffsetBlendSpace1D::StaticClass(), FGdhAssetNamingInfo{TEXT("AO")});
	Mappings.Add(USkeletalMesh::StaticClass(), FGdhAssetNamingInfo{TEXT("SKM")});
	Mappings.Add(UStaticMesh::StaticClass(), FGdhAssetNamingInfo{TEXT("SM")});
	Mappings.Add(USkeleton::StaticClass(), FGdhAssetNamingInfo{TEXT("SK")});
	Mappings.Add(UPoseAsset::StaticClass(), FGdhAssetNamingInfo{TEXT("A"), TEXT("Pose")});
	Mappings.Add(UPhysicsAsset::StaticClass(), FGdhAssetNamingInfo{TEXT("PA")});
	Mappings.Add(UAnimBoneCompressionSettings::StaticClass(), FGdhAssetNamingInfo{TEXT("A"), TEXT("BCS")});
	Mappings.Add(UAnimCurveCompressionSettings::StaticClass(), FGdhAssetNamingInfo{TEXT("A"), TEXT("CCS")});
	Mappings.Add(UAnimBlueprint::StaticClass(), FGdhAssetNamingInfo{TEXT("ABP")});
	Mappings.Add(UAnimLayerInterface::StaticClass(), FGdhAssetNamingInfo{TEXT("ALI")});
	Mappings.Add(ULevelSequence::StaticClass(), FGdhAssetNamingInfo{TEXT("LS")});
	Mappings.Add(UCameraAnim::StaticClass(), FGdhAssetNamingInfo{TEXT("LS")});

	// ai
	Mappings.Add(UBehaviorTree::StaticClass(), FGdhAssetNamingInfo{TEXT("BT")});
	Mappings.Add(UBlackboardData::StaticClass(), FGdhAssetNamingInfo{TEXT("BB")});
	Mappings.Add(UEnvQuery::StaticClass(), FGdhAssetNamingInfo{TEXT("EQS")});
	Mappings.Add(UBTTask_BlueprintBase::StaticClass(), FGdhAssetNamingInfo{TEXT("AI"), TEXT("Task")});
	Mappings.Add(UBTService_BlueprintBase::StaticClass(), FGdhAssetNamingInfo{TEXT("AI"), TEXT("Service")});
	Mappings.Add(UBTDecorator_BlueprintBase::StaticClass(), FGdhAssetNamingInfo{TEXT("AI"), TEXT("Decorator")});

	// materials
	Mappings.Add(UMaterial::StaticClass(), FGdhAssetNamingInfo{TEXT("M")});
	Mappings.Add(UMaterialInstance::StaticClass(), FGdhAssetNamingInfo{TEXT("MI")});
	Mappings.Add(UMaterialInstanceConstant::StaticClass(), FGdhAssetNamingInfo{TEXT("MI")});
	Mappings.Add(UMaterialInstanceDynamic::StaticClass(), FGdhAssetNamingInfo{TEXT("MI")});
	Mappings.Add(UMaterialFunction::StaticClass(), FGdhAssetNamingInfo{TEXT("MF")});
	Mappings.Add(UMaterialParameterCollection::StaticClass(), FGdhAssetNamingInfo{TEXT("MF")});
	Mappings.Add(USubsurfaceProfile::StaticClass(), FGdhAssetNamingInfo{TEXT("SSP")});
	Mappings.Add(UPhysicalMaterial::StaticClass(), FGdhAssetNamingInfo{TEXT("PM")});
	Mappings.Add(UMaterialFunctionMaterialLayer::StaticClass(), FGdhAssetNamingInfo{TEXT("ML")});
	Mappings.Add(UMaterialFunctionMaterialLayerBlend::StaticClass(), FGdhAssetNamingInfo{TEXT("MLB")});

	// textures
	Mappings.Add(UTexture2D::StaticClass(), FGdhAssetNamingInfo{TEXT("T")});
	Mappings.Add(UTextureCube::StaticClass(), FGdhAssetNamingInfo{TEXT("T"), TEXT("HDR")});
	Mappings.Add(UMediaTexture::StaticClass(), FGdhAssetNamingInfo{TEXT("T"), TEXT("Media")});
	Mappings.Add(UVolumeTexture::StaticClass(), FGdhAssetNamingInfo{TEXT("T"), TEXT("Volume")});
	Mappings.Add(URuntimeVirtualTexture::StaticClass(), FGdhAssetNamingInfo{TEXT("T"), TEXT("RVT")});
	Mappings.Add(UTexture2DArray::StaticClass(), FGdhAssetNamingInfo{TEXT("T"), TEXT("2DA")});

	// Render Targets
	Mappings.Add(UTextureRenderTarget2D::StaticClass(), FGdhAssetNamingInfo{TEXT("RT")});
	Mappings.Add(UTextureRenderTarget2DArray::StaticClass(), FGdhAssetNamingInfo{TEXT("RT")});
	Mappings.Add(UCanvasRenderTarget2D::StaticClass(), FGdhAssetNamingInfo{TEXT("RT")});
	Mappings.Add(UTextureRenderTargetCube::StaticClass(), FGdhAssetNamingInfo{TEXT("RT")});
	Mappings.Add(UTextureRenderTargetVolume::StaticClass(), FGdhAssetNamingInfo{TEXT("RT")});

	// fx
	Mappings.Add(UParticleSystem::StaticClass(), FGdhAssetNamingInfo{TEXT("FX")});
	Mappings.Add(UNiagaraScript::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"), TEXT("Script")});
	Mappings.Add(UNiagaraEffectType::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"),TEXT("Type")});
	Mappings.Add(UNiagaraEmitter::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"), TEXT("Emitter")});
	Mappings.Add(UNiagaraSystem::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"), TEXT("System")});
	Mappings.Add(UNiagaraParameterCollection::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"), TEXT("PC")});
	Mappings.Add(UNiagaraParameterCollectionInstance::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"), TEXT("PCI")});
	Mappings.Add(UNiagaraParameterDefinitions::StaticClass(), FGdhAssetNamingInfo{TEXT("FX"), TEXT("Def")});

	// foliage
	Mappings.Add(UFoliageType_Actor::StaticClass(), FGdhAssetNamingInfo{TEXT("FOL")});
	Mappings.Add(UFoliageType_InstancedStaticMesh::StaticClass(), FGdhAssetNamingInfo{TEXT("FOL"), TEXT("ISM")});
	Mappings.Add(ULandscapeGrassType::StaticClass(), FGdhAssetNamingInfo{TEXT("FOL"), TEXT("LGT")});

	// slate
	Mappings.Add(USlateBrushAsset::StaticClass(), FGdhAssetNamingInfo{TEXT("SLT"), TEXT("Brush")});
	Mappings.Add(USlateWidgetStyleAsset::StaticClass(), FGdhAssetNamingInfo{TEXT("SLT"), TEXT("WSA")});
	Mappings.Add(UFont::StaticClass(), FGdhAssetNamingInfo{TEXT("Font")});
	Mappings.Add(USlateVectorArtData::StaticClass(), FGdhAssetNamingInfo{TEXT("SLT"), TEXT("VA")});

	// sound
	Mappings.Add(UDialogueVoice::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("DV")});
	Mappings.Add(UDialogueWave::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("DW")});
	Mappings.Add(USoundClass::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("Class")});
	Mappings.Add(USoundConcurrency::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("Concurrency")});
	Mappings.Add(USoundCue::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("Cue")});
	Mappings.Add(USoundMix::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("Mix")});
	Mappings.Add(USoundWave::StaticClass(), FGdhAssetNamingInfo{TEXT("S"), TEXT("Wave")});
	Mappings.Add(UReverbEffect::StaticClass(), FGdhAssetNamingInfo{TEXT("SX")});
	Mappings.Add(USoundAttenuation::StaticClass(), FGdhAssetNamingInfo{TEXT("SX")});

	// misc
	Mappings.Add(UDataAsset::StaticClass(), FGdhAssetNamingInfo{TEXT("DA")});
	Mappings.Add(UDataTable::StaticClass(), FGdhAssetNamingInfo{TEXT("DT")});
	Mappings.Add(UCurveFloat::StaticClass(), FGdhAssetNamingInfo{TEXT("C"), TEXT("F")});
	Mappings.Add(UCurveVector::StaticClass(), FGdhAssetNamingInfo{TEXT("C"), TEXT("V")});
	Mappings.Add(UCurveLinearColor::StaticClass(), FGdhAssetNamingInfo{TEXT("C"), TEXT("LC")});
	Mappings.Add(UEnum::StaticClass(), FGdhAssetNamingInfo{TEXT("E")});
	Mappings.Add(UUserDefinedEnum::StaticClass(), FGdhAssetNamingInfo{TEXT("E")});
	Mappings.Add(UUserDefinedStruct::StaticClass(), FGdhAssetNamingInfo{TEXT("F")});
}

FName UGdhAssetNamingConvention::GetContainerName() const
{
	return TEXT("Project");
}

FName UGdhAssetNamingConvention::GetCategoryName() const
{
	return TEXT("Plugins");
}

FText UGdhAssetNamingConvention::GetSectionText() const
{
	return FText::FromString(TEXT("GamedevHelper"));
}

#if WITH_EDITOR
void UGdhAssetNamingConvention::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
