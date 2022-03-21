// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingConvention.h"

// Engine Headers
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
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveVector.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/UserDefinedStruct.h"
// Blueprint classes
#include "Blutility/Classes/EditorUtilityBlueprint.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "WidgetBlueprint.h"

UGamedevHelperAssetNamingConvention::UGamedevHelperAssetNamingConvention()
{
	SetDefaultSettings();
}

void UGamedevHelperAssetNamingConvention::GetAssetClasses(TArray<UClass*>& Classes)
{
	Classes.Reserve(BlueprintTypes.Num());

	for (const auto& BlueprintType : BlueprintTypes)
	{
		if (BlueprintType.Value.AssetClass)
		{
			Classes.AddUnique(BlueprintType.Value.AssetClass);
		}
	}

	for (const auto AssetType : Namings)
	{
		const auto NamingSettings = AssetType.Value.Settings;
		Classes.Reserve(Classes.Num() + NamingSettings.Num());
		
		for (const auto& NamingSetting : NamingSettings)
		{
			if (NamingSetting.AssetClass)
			{
				Classes.AddUnique(NamingSetting.AssetClass);
			}
		}
	}
}

FGamedevHelperAssetNameFormat UGamedevHelperAssetNamingConvention::GetAssetNameFormat(const FAssetData& Asset) const
{
	if (!Asset.IsValid())
	{
		return FGamedevHelperAssetNameFormat{};
	}

	const auto AssetClass = Asset.GetClass();

	for (const auto& AssetType : Namings)
	{
		const auto NamingFormat = AssetType.Value.Settings.FindByPredicate([&](const FGamedevHelperAssetNameFormat& Other)
		{
			return Other.AssetClass && Other.AssetClass->GetName().Equals(AssetClass->GetName());
		});

		// if asset is blueprint
		// determine prefix by blueprint type
		// determine suffix by parent class
		

		if (NamingFormat)
		{
			return *NamingFormat;
		}
	}
	
	return FGamedevHelperAssetNameFormat{};
}

#if WITH_EDITOR
void UGamedevHelperAssetNamingConvention::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (OnConventionPropertyChangeDelegate.IsBound())
	{
		OnConventionPropertyChangeDelegate.Execute();
	}
}
#endif

void UGamedevHelperAssetNamingConvention::SetDefaultSettings()
{
	// Blueprint Types
	BlueprintTypes.Add(EGamedevHelperBlueprintType::Normal, FGamedevHelperAssetNameFormat{UBlueprint::StaticClass(), TEXT("BP")});
	BlueprintTypes.Add(EGamedevHelperBlueprintType::Component, FGamedevHelperAssetNameFormat{UBlueprint::StaticClass(), TEXT("BPC")});
	BlueprintTypes.Add(EGamedevHelperBlueprintType::Interface, FGamedevHelperAssetNameFormat{UBlueprint::StaticClass(), TEXT("BPI")});
	BlueprintTypes.Add(EGamedevHelperBlueprintType::FunctionLibrary, FGamedevHelperAssetNameFormat{UBlueprint::StaticClass(), TEXT("BPFL")});
	BlueprintTypes.Add(EGamedevHelperBlueprintType::MacroLibrary, FGamedevHelperAssetNameFormat{UBlueprint::StaticClass(), TEXT("BPML")});

	Namings.Add(EGamedevHelperAssetType::Material, GetMaterialNamings());
	Namings.Add(EGamedevHelperAssetType::Texture, GetTextureNamings());
	Namings.Add(EGamedevHelperAssetType::Animation, GetAnimationNamings());
	Namings.Add(EGamedevHelperAssetType::FX, GetFXNamings());
	Namings.Add(EGamedevHelperAssetType::AI, GetAINamings());
	Namings.Add(EGamedevHelperAssetType::Foliage, GetFoliageNamings());
	Namings.Add(EGamedevHelperAssetType::Slate, GetSlateNamings());
	Namings.Add(EGamedevHelperAssetType::Paper2D, GetPaper2DNamings());
	Namings.Add(EGamedevHelperAssetType::Sound, GetSoundNamings());
	Namings.Add(EGamedevHelperAssetType::Misc, GetMiscNamings());
	Namings.Add(EGamedevHelperAssetType::Blueprint, GetBlueprintNamings());
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetMaterialNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterial::StaticClass(), TEXT("M")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialInstance::StaticClass(), TEXT("MI")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialInstanceConstant::StaticClass(), TEXT("MI")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialInstanceDynamic::StaticClass(), TEXT("MI")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialFunction::StaticClass(), TEXT("MF")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialParameterCollection::StaticClass(), TEXT("MF")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USubsurfaceProfile::StaticClass(), TEXT("SSP")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPhysicalMaterial::StaticClass(), TEXT("M"), TEXT("Phys")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialFunctionMaterialLayer::StaticClass(), TEXT("ML")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMaterialFunctionMaterialLayerBlend::StaticClass(), TEXT("MLB")});

	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetTextureNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTexture2D::StaticClass(), TEXT("T")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTextureCube::StaticClass(), TEXT("T"), TEXT("HDR")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UMediaTexture::StaticClass(), TEXT("T"), TEXT("Media")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UVolumeTexture::StaticClass(), TEXT("T"), TEXT("Volume")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{URuntimeVirtualTexture::StaticClass(), TEXT("T"), TEXT("RVT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTexture2DArray::StaticClass(), TEXT("T"), TEXT("2DA")});

	// Render Targets
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTextureRenderTarget2D::StaticClass(), TEXT("RT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTextureRenderTarget2DArray::StaticClass(), TEXT("RT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UCanvasRenderTarget2D::StaticClass(), TEXT("RT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTextureRenderTargetCube::StaticClass(), TEXT("RT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UTextureRenderTargetVolume::StaticClass(), TEXT("RT")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetAnimationNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimSequence::StaticClass(), TEXT("AN")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimMontage::StaticClass(), TEXT("AM")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimComposite::StaticClass(), TEXT("AC")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBlendSpace::StaticClass(), TEXT("BS")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBlendSpace1D::StaticClass(), TEXT("BS"), TEXT("1D")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAimOffsetBlendSpace::StaticClass(), TEXT("AO")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAimOffsetBlendSpace1D::StaticClass(), TEXT("AO"), TEXT("1D")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USkeletalMesh::StaticClass(), TEXT("SKM")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UStaticMesh::StaticClass(), TEXT("SM")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USkeleton::StaticClass(), TEXT("SK")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPoseAsset::StaticClass(), TEXT("A"), TEXT("Pose")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPhysicsAsset::StaticClass(), TEXT("A"), TEXT("Phys")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{ULevelSequence::StaticClass(), TEXT("LS")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimBoneCompressionSettings::StaticClass(), TEXT("A"), TEXT("BCS")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimCurveCompressionSettings::StaticClass(), TEXT("A"), TEXT("CCS")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimBlueprint::StaticClass(), TEXT("ABP")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UAnimLayerInterface::StaticClass(), TEXT("ALI")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetFXNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UParticleSystem::StaticClass(), TEXT("FX"), TEXT("Cascade")});
	// todo:ashe23 must deduce suffix from usage tag , like blueprints
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraScript::StaticClass(), TEXT("FX"), TEXT("Cascade")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraEffectType::StaticClass(), TEXT("FX"), TEXT("Type")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraEmitter::StaticClass(), TEXT("FX"), TEXT("Emitter")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraSystem::StaticClass(), TEXT("FX"), TEXT("System")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraParameterCollection::StaticClass(), TEXT("FX"), TEXT("PC")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraParameterCollectionInstance::StaticClass(), TEXT("FX"), TEXT("PCI")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UNiagaraParameterDefinitions::StaticClass(), TEXT("FX"), TEXT("DEF")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetAINamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBehaviorTree::StaticClass(), TEXT("BT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBlackboardData::StaticClass(), TEXT("BB")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UEnvQuery::StaticClass(), TEXT("EQS")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBTTask_BlueprintBase::StaticClass(), TEXT("AI"), TEXT("Task")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBTService_BlueprintBase::StaticClass(), TEXT("AI"), TEXT("Service")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UBTDecorator_BlueprintBase::StaticClass(), TEXT("AI"), TEXT("Decorator")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetFoliageNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UFoliageType_Actor::StaticClass(), TEXT("FOL")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UFoliageType_InstancedStaticMesh::StaticClass(), TEXT("FOL"), TEXT("ISM")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{ULandscapeGrassType::StaticClass(), TEXT("FOL"), TEXT("LGT")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetSlateNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USlateBrushAsset::StaticClass(), TEXT("SLT"), TEXT("Brush")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USlateWidgetStyleAsset::StaticClass(), TEXT("SLT"), TEXT("WSA")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UFont::StaticClass(), TEXT("Font")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetPaper2DNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPaperFlipbook::StaticClass(), TEXT("P2"), TEXT("Flipbook")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPaperSprite::StaticClass(), TEXT("P2"), TEXT("Sprite")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPaperTileMap::StaticClass(), TEXT("P2"), TEXT("TileMap")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UPaperTileSet::StaticClass(), TEXT("P2"), TEXT("TileSet")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetSoundNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UDialogueVoice::StaticClass(), TEXT("S"), TEXT("DV")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UDialogueWave::StaticClass(), TEXT("S"), TEXT("DW")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USoundClass::StaticClass(), TEXT("S"), TEXT("Class")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USoundConcurrency::StaticClass(), TEXT("S"), TEXT("Concurrency")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USoundCue::StaticClass(), TEXT("S"), TEXT("Cue")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USoundMix::StaticClass(), TEXT("S"), TEXT("Mix")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USoundWave::StaticClass(), TEXT("S"), TEXT("Wave")});

	// Sound Effects
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UReverbEffect::StaticClass(), TEXT("SX"), TEXT("Reverb")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{USoundAttenuation::StaticClass(), TEXT("SX"), TEXT("Attenuation")});
	// todo:ashe23 update list
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetMiscNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UDataAsset::StaticClass(), TEXT("DA")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UDataTable::StaticClass(), TEXT("DT")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UCurveFloat::StaticClass(), TEXT("C"), TEXT("F")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UCurveVector::StaticClass(), TEXT("C"), TEXT("V")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UCurveLinearColor::StaticClass(), TEXT("C"), TEXT("LC")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UEnum::StaticClass(), TEXT("E")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UUserDefinedEnum::StaticClass(), TEXT("E")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UUserDefinedStruct::StaticClass(), TEXT("F")});
	
	return AssetNamings;
}

FGamedevHelperAssetNamings UGamedevHelperAssetNamingConvention::GetBlueprintNamings()
{
	FGamedevHelperAssetNamings AssetNamings;
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UEditorUtilityBlueprint::StaticClass(), TEXT("BP"), TEXT("EUB")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UEditorUtilityWidgetBlueprint::StaticClass(), TEXT("BP"), TEXT("EUW")});
	AssetNamings.Settings.Add(FGamedevHelperAssetNameFormat{UWidgetBlueprint::StaticClass(), TEXT("BPW")});
	
	return AssetNamings;
}
