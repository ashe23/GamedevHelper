// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"

#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"

// Material classes
#include "Materials/Material.h"
// #include "Materials/MaterialInstance.h"
// #include "Materials/MaterialInstanceConstant.h"
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
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/TextureRenderTarget2DArray.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Engine/TextureRenderTargetVolume.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/VolumeTexture.h"
#include "MediaTexture.h"
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
// FX
#include "Particles/ParticleSystem.h"
// #include "NiagaraCommon.h"
#include "NiagaraEffectType.h"
#include "NiagaraScript.h"
#include "NiagaraEmitter.h"
#include "NiagaraParameterCollection.h"
#include "NiagaraSystem.h"
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
// Sound
#include "Sound/DialogueVoice.h"
#include "Sound/DialogueWave.h"
#include "Sound/ReverbEffect.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundConcurrency.h"
// Slate
#include "Slate/SlateBrushAsset.h"
#include "Styling/SlateWidgetStyleAsset.h"
// Misc
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Engine/Font.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveVector.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/UserDefinedStruct.h"
// Blueprint classes
// #include "Engine/Blueprint.h"
// #include "Kismet/BlueprintFunctionLibrary.h"
// #include "Components/Widget.h"
#include "UObject/Class.h"
#include "WidgetBlueprint.h"
// EditorScriptingUtilities classes
#include "Blutility/Classes/EditorUtilityBlueprint.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"
// Paper2D
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"

UGamedevHelperAssetNamingManagerSettings::UGamedevHelperAssetNamingManagerSettings()
{
	SetDefaultSettings();
}

void UGamedevHelperAssetNamingManagerSettings::SetDefaultSettings()
{
	ScanPath.Path = TEXT("/Game");
	NamingCase = EGamedevHelperNamingCase::PascalSnakeCase;
	bIgnoreNamingCaseOnPrefixes = true;
	bIgnoreNamingCaseOnSuffixes = true;

	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::Normal, FGamedevHelperAssetNameSettings{TEXT("BP")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::Interface, FGamedevHelperAssetNameSettings{TEXT("BPI")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::FunctionLibrary, FGamedevHelperAssetNameSettings{TEXT("BFL")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::MacroLibrary, FGamedevHelperAssetNameSettings{TEXT("BML")});

	// Materials
	AssetsNaming.Add(UMaterial::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("M")});
	AssetsNaming.Add(UMaterialInstance::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MI")});
	AssetsNaming.Add(UMaterialInstanceDynamic::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MI")});
	AssetsNaming.Add(UMaterialInstanceConstant::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MI")});
	AssetsNaming.Add(UMaterialFunction::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MF")});
	AssetsNaming.Add(UMaterialParameterCollection::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MPC")});
	AssetsNaming.Add(USubsurfaceProfile::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SSP")});
	AssetsNaming.Add(UPhysicalMaterial::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("M"), TEXT("Phys")});
	AssetsNaming.Add(UMaterialFunctionMaterialLayer::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ML")});
	AssetsNaming.Add(UMaterialFunctionMaterialLayerBlend::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MLB")});
	
	// Textures
	AssetsNaming.Add(UTexture2D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("T")});
	AssetsNaming.Add(UTextureCube::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("HDR")});
	AssetsNaming.Add(UTextureRenderTarget2D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	AssetsNaming.Add(UTextureRenderTarget2DArray::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	AssetsNaming.Add(UCanvasRenderTarget2D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	AssetsNaming.Add(UTextureRenderTargetCube::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	AssetsNaming.Add(UTextureRenderTargetVolume::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	AssetsNaming.Add(UMediaTexture::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MT")});
	AssetsNaming.Add(UVolumeTexture::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("VT")});

	// Animations
	AssetsNaming.Add(UAnimSequence::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AN")});
	AssetsNaming.Add(UAnimMontage::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AM")});
	AssetsNaming.Add(UAnimComposite::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AC")});
	AssetsNaming.Add(UBlendSpace::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BC")});
	AssetsNaming.Add(UBlendSpace1D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BC"), TEXT("1D")});
	AssetsNaming.Add(UAimOffsetBlendSpace::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AO")});
	AssetsNaming.Add(UAimOffsetBlendSpace1D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AO"), TEXT("1D")});
	AssetsNaming.Add(USkeletalMesh::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SKM")});
	AssetsNaming.Add(USkeleton::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SK")});
	AssetsNaming.Add(UPoseAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Pose")});
	AssetsNaming.Add(UPhysicsAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PA")});
	AssetsNaming.Add(ULevelSequence::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("LS")});
	AssetsNaming.Add(UAnimBoneCompressionSettings::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ABCS")});
	AssetsNaming.Add(UAnimCurveCompressionSettings::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ACCS")});

	// FX
	AssetsNaming.Add(UParticleSystem::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PS")});
	AssetsNaming.Add(UNiagaraScript::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NS")}); // todo:ashe23 must deduce suffix from usage tag , like blueprints
	AssetsNaming.Add(UNiagaraEffectType::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NFT")});
	AssetsNaming.Add(UNiagaraEmitter::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NE")});
	AssetsNaming.Add(UNiagaraParameterCollection::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPC")});
	AssetsNaming.Add(UNiagaraParameterCollectionInstance::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPCI")});
	AssetsNaming.Add(UNiagaraParameterDefinitionsBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPD")});
	AssetsNaming.Add(UNiagaraSystem::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NS")});
	
	// AI
	AssetsNaming.Add(UBehaviorTree::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT")});
	AssetsNaming.Add(UBlackboardData::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BB")});
	AssetsNaming.Add(UEnvQuery::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("EQS")});
	AssetsNaming.Add(UBTTask_BlueprintBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT"), TEXT("Task")});
	AssetsNaming.Add(UBTService_BlueprintBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT"), TEXT("Service")});
	AssetsNaming.Add(UBTDecorator_BlueprintBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT"), TEXT("Decorator")});

	// Foliage
	AssetsNaming.Add(UFoliageType_Actor::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("FTA")});
	AssetsNaming.Add(UFoliageType_InstancedStaticMesh::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("FSM")});
	AssetsNaming.Add(ULandscapeGrassType::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("FGT")});

	// Sounds
	AssetsNaming.Add(UDialogueVoice::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SDV")});
	AssetsNaming.Add(UDialogueWave::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SDW")});
	AssetsNaming.Add(UReverbEffect::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SRE")});
	AssetsNaming.Add(USoundAttenuation::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SATT")});
	AssetsNaming.Add(USoundClass::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("S"), TEXT("Class")});
	AssetsNaming.Add(USoundConcurrency::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SCC")});
	AssetsNaming.Add(USoundCue::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SC")});
	AssetsNaming.Add(USoundMix::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("S"), TEXT("Mix")});
	AssetsNaming.Add(USoundWave::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SW")});

	// Slate
	AssetsNaming.Add(USlateBrushAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Brush")});
	AssetsNaming.Add(USlateWidgetStyleAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Style")});

	// Misc
	AssetsNaming.Add(UDataAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("DA")});
	AssetsNaming.Add(UDataTable::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("DT")});
	AssetsNaming.Add(UCurveFloat::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("C"), TEXT("Float")});
	AssetsNaming.Add(UCurveVector::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("C"), TEXT("Vector")});
	AssetsNaming.Add(UCurveLinearColor::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("C"), TEXT("Color")});
	AssetsNaming.Add(UFont::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Font")});
	AssetsNaming.Add(UStaticMesh::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SM")});
	AssetsNaming.Add(UEnum::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("E")});
	AssetsNaming.Add(UUserDefinedEnum::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("E")});
	AssetsNaming.Add(UUserDefinedStruct::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("F")});

	// Blueprint classes
	AssetsNaming.Add(UAnimBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ABP")});
	AssetsNaming.Add(UAnimLayerInterface::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ALI")});
	AssetsNaming.Add(UWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("WBP")});

	// EditorScriptingUtilities
	AssetsNaming.Add(UEditorUtilityBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("EUB")});
	AssetsNaming.Add(UEditorUtilityWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("EUW")});

	// Paper2D
	AssetsNaming.Add(UPaperFlipbook::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperFB")});
	AssetsNaming.Add(UPaperSprite::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperS")});
	AssetsNaming.Add(UPaperTileMap::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperTM")});
	AssetsNaming.Add(UPaperTileSet::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperTS")});
}
