// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"

#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"

// Material classes
#include "Materials/Material.h"
// #include "Materials/MaterialInstance.h"
// #include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
// #include "Materials/MaterialFunction.h"
// #include "Materials/MaterialParameterCollection.h"
// #include "Engine/Classes/Materials/MaterialFunctionMaterialLayer.h"
// #include "Engine/Classes/Materials/MaterialFunctionMaterialLayerBlend.h"
// #include "Engine/SubsurfaceProfile.h"
// #include "PhysicalMaterials/PhysicalMaterial.h"
// // Texture classes
// #include "Engine/Texture2D.h"
// #include "Engine/TextureCube.h"
// #include "Engine/TextureRenderTarget2D.h"
// #include "Engine/TextureRenderTarget2DArray.h"
// #include "Engine/TextureRenderTargetCube.h"
// #include "Engine/TextureRenderTargetVolume.h"
// #include "Engine/CanvasRenderTarget2D.h"
// #include "Engine/VolumeTexture.h"
// #include "MediaTexture.h"
// // Animation classes
// #include "Animation/AnimSequence.h"
// #include "Animation/AnimBlueprint.h"
// #include "Animation/AnimMontage.h"
// #include "Animation/AnimComposite.h"
// #include "Animation/BlendSpace.h"
// #include "Animation/BlendSpace1D.h"
// #include "Animation/Skeleton.h"
// #include "Animation/PoseAsset.h"
// #include "Animation/AnimLayerInterface.h"
// #include "Animation/AimOffsetBlendSpace.h"
// #include "Animation/AimOffsetBlendSpace1D.h"
// #include "Animation/AnimBoneCompressionSettings.h"
// #include "Animation/AnimCurveCompressionSettings.h"
// #include "Engine/SkeletalMesh.h"
// #include "PhysicsEngine/PhysicsAsset.h"
// #include "LevelSequence.h"
// // FX
// #include "Particles/ParticleSystem.h"
// // AI
// #include "BehaviorTree/BehaviorTree.h"
// #include "BehaviorTree/BlackboardData.h"
// #include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
// #include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
// #include "BehaviorTree/Services/BTService_BlueprintBase.h"
// #include "EnvironmentQuery/EnvQuery.h"
// // Foliage
// #include "FoliageType_Actor.h"
// #include "FoliageType_InstancedStaticMesh.h"
// #include "LandscapeGrassType.h"
// // Sound
// #include "Sound/DialogueVoice.h"
// #include "Sound/DialogueWave.h"
// #include "Sound/ReverbEffect.h"
// #include "Sound/SoundClass.h"
// #include "Sound/SoundCue.h"
// #include "Sound/SoundMix.h"
// #include "Sound/SoundWave.h"
// #include "Sound/SoundConcurrency.h"
// // Slate
// #include "Slate/SlateBrushAsset.h"
// #include "Styling/SlateWidgetStyleAsset.h"
// // Misc
// #include "Engine/DataAsset.h"
// #include "Engine/DataTable.h"
// #include "Engine/Font.h"
// #include "Curves/CurveLinearColor.h"
// #include "Curves/CurveVector.h"
// #include "Engine/UserDefinedEnum.h"
// #include "Engine/UserDefinedStruct.h"
// // Blueprint classes
// #include "Engine/Blueprint.h"
// #include "Kismet/BlueprintFunctionLibrary.h"
// // #include "Components/Widget.h"
// #include "UObject/Class.h"
// #include "WidgetBlueprint.h"

UGamedevHelperAssetNamingManagerSettings::UGamedevHelperAssetNamingManagerSettings()
{
	SetDefaultSettings();
}

void UGamedevHelperAssetNamingManagerSettings::SetDefaultSettings()
{
	ScanPath.Path = TEXT("/Game");
	NamingCase = EGamedevHelperNamingCase::PascalSnakeCase;
	bIgnoreNamingCaseOnPrefixes = false;
	bIgnoreNamingCaseOnSuffixes = false;

	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::Normal, FGamedevHelperAssetName{TEXT("BP")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::Interface, FGamedevHelperAssetName{TEXT("BPI")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::FunctionLibrary, FGamedevHelperAssetName{TEXT("BFL")});
	BlueprintTypesNaming.Add(EGamedevHelperBlueprintType::MacroLibrary, FGamedevHelperAssetName{TEXT("BML")});

	AssetsNaming.Add(UMaterial::StaticClass(), FGamedevHelperAssetName{TEXT("M")});
	AssetsNaming.Add(UMaterialInstance::StaticClass(), FGamedevHelperAssetName{TEXT("MI")});
	AssetsNaming.Add(UMaterialInstanceDynamic::StaticClass(), FGamedevHelperAssetName{TEXT("MI")});
	AssetsNaming.Add(UMaterialInstanceConstant::StaticClass(), FGamedevHelperAssetName{TEXT("MI")});
}
