// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingConvention.h"
#include "GamedevHelperAssetLibrary.h"
// Engine Headers
// Blueprint classes
#include "EditorUtilityWidgetBlueprint.h"
#include "Blutility/Classes/EditorUtilityBlueprint.h"
#include "WidgetBlueprint.h"
#include "GameFramework/Character.h"
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

UGamedevHelperAssetNamingConvention::UGamedevHelperAssetNamingConvention()
{
	SetDefaultSettings();
}

FGamedevHelperAssetNameFormat UGamedevHelperAssetNamingConvention::GetAssetNameFormat(const FAssetData& Asset) const
{
	if (!Asset.IsValid())
	{
		return FGamedevHelperAssetNameFormat{};
	}

	return GetNaming(Asset);
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

FGamedevHelperAssetNameFormat UGamedevHelperAssetNamingConvention::GetNaming(const FAssetData& Asset) const
{
	if (!Asset.IsValid())
	{
		return FGamedevHelperAssetNameFormat{};
	}

	if (!Asset.AssetClass.IsEqual(TEXT("Blueprint")))
	{
		return GetNamingByClass(Asset.GetClass());
	}

	const UClass* ParentClass = UGamedevHelperAssetLibrary::GetBlueprintParentClass(Asset);
	const UClass* SearchClass = ParentClass ? ParentClass : UBlueprint::StaticClass();
	FGamedevHelperAssetNameFormat NameFormat = GetNamingByClass(SearchClass);

	if (NameFormat.Prefix.IsEmpty())
	{
		const auto BlueprintType = UGamedevHelperAssetLibrary::GetBlueprintType(Asset);
		const auto BlueprintTypePrefix = BlueprintTypePrefixes.Find(BlueprintType);
		const FString BlueprintPrefix = BlueprintTypePrefix ? *BlueprintTypePrefix : TEXT("BP");
		NameFormat.Prefix = BlueprintPrefix;
	}

	if (NameFormat.bAssetClassAsSuffix)
	{
		NameFormat.Suffix = ParentClass ? ParentClass->GetName() : Asset.GetClass()->GetName();
	}

	return NameFormat;
}

FGamedevHelperAssetNameFormat UGamedevHelperAssetNamingConvention::GetNamingByClass(const UClass* SearchClass) const
{
	if (!SearchClass)
	{
		return FGamedevHelperAssetNameFormat{};
	}

	for (const auto& Naming : Namings)
	{
		const UClass* NamingClass = Naming.Key;
		if (NamingClass == SearchClass)
		{
			return Naming.Value;
		}
	}

	return FGamedevHelperAssetNameFormat{};
}

void UGamedevHelperAssetNamingConvention::SetDefaultSettings()
{
	SetLevelNamings();
	SetBlueprintNamings();
	SetAnimationNamings();
	SetAINamings();
	SetMaterialNamings();
	SetTextureNamings();
	SetFXNamings();
	SetFoliageNamings();
	SetPaper2DNamings();
	SetSlateNamings();
	SetSoundNamings();
	SetMiscNamings();
}

void UGamedevHelperAssetNamingConvention::SetLevelNamings()
{
	Namings.Add(UWorld::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("LVL")});
}

void UGamedevHelperAssetNamingConvention::SetBlueprintNamings()
{
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::Normal,TEXT("BP"));
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::Interface,TEXT("BPI"));
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::FunctionLibrary,TEXT("BPFL"));
	BlueprintTypePrefixes.Add(EGamedevHelperBlueprintType::MacroLibrary,TEXT("BPML"));

	Namings.Add(UBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP")});
	Namings.Add(AActor::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP")});
	Namings.Add(UActorComponent::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Comp")});
	Namings.Add(APawn::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Pawn")});
	Namings.Add(ACharacter::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Char")});
	Namings.Add(APlayerController::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Ctrl")});
	Namings.Add(AGameModeBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("GM")});
	Namings.Add(UWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BPW")});
	Namings.Add(UEditorTutorial::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("Tut")});
	Namings.Add(UEditorUtilityBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("EUB")});
	Namings.Add(UEditorUtilityWidgetBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BP"), TEXT("EUW")});
}

void UGamedevHelperAssetNamingConvention::SetAnimationNamings()
{
	Namings.Add(UAnimSequence::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AN")});
	Namings.Add(UAnimMontage::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AM")});
	Namings.Add(UAnimComposite::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AC")});
	Namings.Add(UBlendSpace::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ABS")});
	Namings.Add(UBlendSpace1D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ABS")});
	Namings.Add(UAimOffsetBlendSpace::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AO")});
	Namings.Add(UAimOffsetBlendSpace1D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AO")});
	Namings.Add(USkeletalMesh::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SKM")});
	Namings.Add(UStaticMesh::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SM")});
	Namings.Add(USkeleton::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SK")});
	Namings.Add(UPoseAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("A"), TEXT("Pose")});
	Namings.Add(UPhysicsAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("PA")});
	Namings.Add(UAnimBoneCompressionSettings::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("A"), TEXT("BCS")});
	Namings.Add(UAnimCurveCompressionSettings::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("A"), TEXT("CCS")});
	Namings.Add(UAnimBlueprint::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ABP")});
	Namings.Add(UAnimLayerInterface::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ALI")});
	Namings.Add(ULevelSequence::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("LS")});
	Namings.Add(UCameraAnim::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("LS")});
}

void UGamedevHelperAssetNamingConvention::SetAINamings()
{
	Namings.Add(UBehaviorTree::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BT")});
	Namings.Add(UBlackboardData::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("BB")});
	Namings.Add(UEnvQuery::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("EQS")});
	Namings.Add(UBTTask_BlueprintBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AI"), TEXT("Task")});
	Namings.Add(UBTService_BlueprintBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AI"), TEXT("Service")});
	Namings.Add(UBTDecorator_BlueprintBase::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("AI"), TEXT("Decorator")});
}

void UGamedevHelperAssetNamingConvention::SetMaterialNamings()
{
	Namings.Add(UMaterial::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("M")});
	Namings.Add(UMaterialInstance::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MI")});
	Namings.Add(UMaterialInstanceConstant::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MI")});
	Namings.Add(UMaterialInstanceDynamic::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MI")});
	Namings.Add(UMaterialFunction::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MF")});
	Namings.Add(UMaterialParameterCollection::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MF")});
	Namings.Add(USubsurfaceProfile::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SSP")});
	Namings.Add(UPhysicalMaterial::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("PM")});
	Namings.Add(UMaterialFunctionMaterialLayer::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("ML")});
	Namings.Add(UMaterialFunctionMaterialLayerBlend::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("MLB")});
}


void UGamedevHelperAssetNamingConvention::SetTextureNamings()
{
	Namings.Add(UTexture2D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T")});
	Namings.Add(UTextureCube::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("HDR")});
	Namings.Add(UMediaTexture::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("Media")});
	Namings.Add(UVolumeTexture::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("Volume")});
	Namings.Add(URuntimeVirtualTexture::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("RVT")});
	Namings.Add(UTexture2DArray::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("T"), TEXT("2DA")});

	// Render Targets
	Namings.Add(UTextureRenderTarget2D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Namings.Add(UTextureRenderTarget2DArray::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Namings.Add(UCanvasRenderTarget2D::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Namings.Add(UTextureRenderTargetCube::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
	Namings.Add(UTextureRenderTargetVolume::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("RT")});
}

void UGamedevHelperAssetNamingConvention::SetFXNamings()
{
	Namings.Add(UParticleSystem::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX")});
	Namings.Add(UNiagaraScript::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("Script")});
	Namings.Add(UNiagaraEffectType::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"),TEXT("Type")});
	Namings.Add(UNiagaraEmitter::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("Emitter")});
	Namings.Add(UNiagaraSystem::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("System")});
	Namings.Add(UNiagaraParameterCollection::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("PC")});
	Namings.Add(UNiagaraParameterCollectionInstance::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("PCI")});
	Namings.Add(UNiagaraParameterDefinitions::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FX"), TEXT("Def")});
}

void UGamedevHelperAssetNamingConvention::SetFoliageNamings()
{
	Namings.Add(UFoliageType_Actor::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FOL")});
	Namings.Add(UFoliageType_InstancedStaticMesh::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FOL"), TEXT("ISM")});
	Namings.Add(ULandscapeGrassType::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("FOL"), TEXT("LGT")});
}

void UGamedevHelperAssetNamingConvention::SetPaper2DNamings()
{
	Namings.Add(UPaperFlipbook::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("Flipbook")});
	Namings.Add(UPaperSprite::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("Sprite")});
	Namings.Add(UPaperTileMap::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("TileMap")});
	Namings.Add(UPaperTileSet::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("P2"), TEXT("TileSet")});
}

void UGamedevHelperAssetNamingConvention::SetSlateNamings()
{
	Namings.Add(USlateBrushAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SLT"), TEXT("Brush")});
	Namings.Add(USlateWidgetStyleAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SLT"), TEXT("WSA")});
	Namings.Add(UFont::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("Font")});
}

void UGamedevHelperAssetNamingConvention::SetSoundNamings()
{
	Namings.Add(UDialogueVoice::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("DV")});
	Namings.Add(UDialogueWave::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("DW")});
	Namings.Add(USoundClass::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Class")});
	Namings.Add(USoundConcurrency::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Concurrency")});
	Namings.Add(USoundCue::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Cue")});
	Namings.Add(USoundMix::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Mix")});
	Namings.Add(USoundWave::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("S"), TEXT("Wave")});

	// Sound Effects
	Namings.Add(UReverbEffect::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SX")});
	Namings.Add(USoundAttenuation::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("SX")});
	// todo:ashe23 update list
}

void UGamedevHelperAssetNamingConvention::SetMiscNamings()
{
	Namings.Add(UDataAsset::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("DA")});
	Namings.Add(UDataTable::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("DT")});
	Namings.Add(UCurveFloat::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("C"), TEXT("F")});
	Namings.Add(UCurveVector::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("C"), TEXT("V")});
	Namings.Add(UCurveLinearColor::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("C"), TEXT("LC")});
	Namings.Add(UEnum::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("E")});
	Namings.Add(UUserDefinedEnum::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("E")});
	Namings.Add(UUserDefinedStruct::StaticClass(), FGamedevHelperAssetNameFormat{TEXT("F")});
}
