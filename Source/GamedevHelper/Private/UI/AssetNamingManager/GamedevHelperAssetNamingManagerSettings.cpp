// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"
#include "GamedevHelper.h"
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

#if WITH_EDITOR
void UGamedevHelperAssetNamingManagerSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (OnSettingsChangeDelegate.IsBound())
	{
		OnSettingsChangeDelegate.Execute();
	}
}
#endif

const FGamedevHelperAssetNameSettings* UGamedevHelperAssetNamingManagerSettings::FindNamingByClass(const UClass* AssetClass) const
{
	// TMap<UClass*, FGamedevHelperAssetNameSettings> AllNamings;

	if (MaterialTypeAssets.Contains(AssetClass))
	{
		return MaterialTypeAssets.Find(AssetClass);
	}
	if (TextureTypeAssets.Contains(AssetClass))
	{
		return TextureTypeAssets.Find(AssetClass);
	}
	if (AnimationTypeAssets.Contains(AssetClass))
	{
		return AnimationTypeAssets.Find(AssetClass);
	}
	if (FXTypeAssets.Contains(AssetClass))
	{
		return FXTypeAssets.Find(AssetClass);
	}
	if (AITypeAssets.Contains(AssetClass))
	{
		return AITypeAssets.Find(AssetClass);
	}
	if (FoliageTypeAssets.Contains(AssetClass))
	{
		return FoliageTypeAssets.Find(AssetClass);
	}
	if (SlateTypeAssets.Contains(AssetClass))
	{
		return SlateTypeAssets.Find(AssetClass);
	}
	if (SoundTypeAssets.Contains(AssetClass))
	{
		return SoundTypeAssets.Find(AssetClass);
	}
	if (Paper2DTypeAssets.Contains(AssetClass))
	{
		return Paper2DTypeAssets.Find(AssetClass);
	}
	if (BlueprintTypeAssets.Contains(AssetClass))
	{
		return BlueprintTypeAssets.Find(AssetClass);
	}
	if (EditorScriptingUtilitiesTypeAssets.Contains(AssetClass))
	{
		return EditorScriptingUtilitiesTypeAssets.Find(AssetClass);
	}
	if (MiscTypeAssets.Contains(AssetClass))
	{
		return MiscTypeAssets.Find(AssetClass);
	}
	if (CustomTypeAssets.Contains(AssetClass))
	{
		return CustomTypeAssets.Find(AssetClass);
	}
	// AllNamings.Append(MaterialTypeAssets);
	// AllNamings.Append(TextureTypeAssets);
	// AllNamings.Append(AnimationTypeAssets);
	// AllNamings.Append(FXTypeAssets);
	// AllNamings.Append(AITypeAssets);
	// AllNamings.Append(FoliageTypeAssets);
	// AllNamings.Append(SoundTypeAssets);
	// AllNamings.Append(SlateTypeAssets);
	// AllNamings.Append(Paper2DTypeAssets);
	// AllNamings.Append(BlueprintTypeAssets);
	// AllNamings.Append(EditorScriptingUtilitiesTypeAssets);
	// AllNamings.Append(MiscTypeAssets);
	// AllNamings.Append(CustomTypeAssets);

	// return AllNamings.Find(AssetClass);
	return nullptr;
}

void UGamedevHelperAssetNamingManagerSettings::SetDefaultSettings()
{
	ScanPath.Path = TEXT("/Game");
	NamingCase = EGamedevHelperNamingCase::PascalSnakeCase;
	// bIgnoreNamingCaseOnPrefixes = true;
	// bIgnoreNamingCaseOnSuffixes = true;

	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::Normal, FGamedevHelperAssetNameSettings{TEXT("BP")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::Interface, FGamedevHelperAssetNameSettings{TEXT("BPI")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::FunctionLibrary, FGamedevHelperAssetNameSettings{TEXT("BFL")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::MacroLibrary, FGamedevHelperAssetNameSettings{TEXT("BML")});

	// Blueprint classes
	BlueprintTypeAssets.Add(UAnimBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ABP")});
	BlueprintTypeAssets.Add(UAnimLayerInterface::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ALI")});
	BlueprintTypeAssets.Add(UWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("WBP")});

	// Materials
	MaterialTypeAssets.Add(UMaterial::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("M")});
	MaterialTypeAssets.Add(UMaterialInstance::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MI")});
	MaterialTypeAssets.Add(UMaterialInstanceDynamic::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MI")});
	MaterialTypeAssets.Add(UMaterialInstanceConstant::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MI")});
	MaterialTypeAssets.Add(UMaterialFunction::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MF")});
	MaterialTypeAssets.Add(UMaterialParameterCollection::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MPC")});
	MaterialTypeAssets.Add(USubsurfaceProfile::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SSP")});
	MaterialTypeAssets.Add(UPhysicalMaterial::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("M"), TEXT("Phys")});
	MaterialTypeAssets.Add(UMaterialFunctionMaterialLayer::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ML")});
	MaterialTypeAssets.Add(UMaterialFunctionMaterialLayerBlend::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MLB")});

	// Textures
	TextureTypeAssets.Add(UTexture2D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("T")});
	TextureTypeAssets.Add(UTextureCube::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("HDR")});
	TextureTypeAssets.Add(UTextureRenderTarget2D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	TextureTypeAssets.Add(UTextureRenderTarget2DArray::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	TextureTypeAssets.Add(UCanvasRenderTarget2D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	TextureTypeAssets.Add(UTextureRenderTargetCube::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	TextureTypeAssets.Add(UTextureRenderTargetVolume::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("RT")});
	TextureTypeAssets.Add(UMediaTexture::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("MT")});
	TextureTypeAssets.Add(UVolumeTexture::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("VT")});

	// Animations
	// todo:ashe23 suffixes not detected correctly
	AnimationTypeAssets.Add(UAnimSequence::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AN")});
	AnimationTypeAssets.Add(UAnimMontage::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AM")});
	AnimationTypeAssets.Add(UAnimComposite::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AC")});
	AnimationTypeAssets.Add(UBlendSpace::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BC")});
	AnimationTypeAssets.Add(UBlendSpace1D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BC"), TEXT("1D")});
	AnimationTypeAssets.Add(UAimOffsetBlendSpace::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AO")});
	AnimationTypeAssets.Add(UAimOffsetBlendSpace1D::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("AO"), TEXT("1D")});
	AnimationTypeAssets.Add(USkeletalMesh::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SKM")});
	AnimationTypeAssets.Add(USkeleton::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SK")});
	AnimationTypeAssets.Add(UPoseAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Pose")});
	AnimationTypeAssets.Add(UPhysicsAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PA")});
	AnimationTypeAssets.Add(ULevelSequence::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("LS")});
	AnimationTypeAssets.Add(UAnimBoneCompressionSettings::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ABCS")});
	AnimationTypeAssets.Add(UAnimCurveCompressionSettings::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("ACCS")});

	// FX
	FXTypeAssets.Add(UParticleSystem::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PS")});
	FXTypeAssets.Add(UNiagaraScript::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NS")});
	// todo:ashe23 must deduce suffix from usage tag , like blueprints
	FXTypeAssets.Add(UNiagaraEffectType::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NFT")});
	FXTypeAssets.Add(UNiagaraEmitter::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NE")});
	FXTypeAssets.Add(UNiagaraParameterCollection::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPC")});
	FXTypeAssets.Add(UNiagaraParameterCollectionInstance::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPCI")});
	FXTypeAssets.Add(UNiagaraParameterDefinitionsBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPD")});
	FXTypeAssets.Add(UNiagaraParameterDefinitions::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NPD")});
	FXTypeAssets.Add(UNiagaraSystem::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("NS")});

	// AI
	AITypeAssets.Add(UBehaviorTree::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT")});
	AITypeAssets.Add(UBlackboardData::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BB")});
	AITypeAssets.Add(UEnvQuery::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("EQS")});
	AITypeAssets.Add(UBTTask_BlueprintBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT"), TEXT("Task")});
	AITypeAssets.Add(UBTService_BlueprintBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT"), TEXT("Service")});
	AITypeAssets.Add(UBTDecorator_BlueprintBase::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("BT"), TEXT("Decorator")});

	// Foliage
	FoliageTypeAssets.Add(UFoliageType_Actor::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("FTA")});
	FoliageTypeAssets.Add(UFoliageType_InstancedStaticMesh::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("FSM")});
	FoliageTypeAssets.Add(ULandscapeGrassType::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("FGT")});

	// Sounds
	SoundTypeAssets.Add(UDialogueVoice::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SDV")});
	SoundTypeAssets.Add(UDialogueWave::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SDW")});
	SoundTypeAssets.Add(UReverbEffect::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SRE")});
	SoundTypeAssets.Add(USoundAttenuation::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SATT")});
	SoundTypeAssets.Add(USoundClass::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("S"), TEXT("Class")});
	SoundTypeAssets.Add(USoundConcurrency::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SCC")});
	SoundTypeAssets.Add(USoundCue::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SC")});
	SoundTypeAssets.Add(USoundMix::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("S"), TEXT("Mix")});
	SoundTypeAssets.Add(USoundWave::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SW")});

	// Slate
	SlateTypeAssets.Add(USlateBrushAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Brush")});
	SlateTypeAssets.Add(USlateWidgetStyleAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Style")});
	SlateTypeAssets.Add(UFont::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("Font")});

	// Paper2D
	// todo:ashe23 prefixes not detected correctly
	Paper2DTypeAssets.Add(UPaperFlipbook::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperFB")});
	Paper2DTypeAssets.Add(UPaperSprite::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperS")});
	Paper2DTypeAssets.Add(UPaperTileMap::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperTM")});
	Paper2DTypeAssets.Add(UPaperTileSet::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("PaperTS")});

	// EditorScriptingUtilities
	EditorScriptingUtilitiesTypeAssets.Add(UEditorUtilityBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("EUB")});
	EditorScriptingUtilitiesTypeAssets.Add(UEditorUtilityWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("EUW")});

	// Misc
	// todo:ashe23 data asset must be more specific
	MiscTypeAssets.Add(UDataAsset::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("DA")});
	MiscTypeAssets.Add(UDataTable::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("DT")});
	MiscTypeAssets.Add(UCurveFloat::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("C"), TEXT("Float")});
	MiscTypeAssets.Add(UCurveVector::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("C"), TEXT("Vector")});
	MiscTypeAssets.Add(UCurveLinearColor::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("C"), TEXT("Color")});
	MiscTypeAssets.Add(UStaticMesh::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("SM")});
	MiscTypeAssets.Add(UEnum::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("E")});
	MiscTypeAssets.Add(UUserDefinedEnum::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("E")});
	MiscTypeAssets.Add(UUserDefinedStruct::StaticClass(), FGamedevHelperAssetNameSettings{TEXT("F")});
}
