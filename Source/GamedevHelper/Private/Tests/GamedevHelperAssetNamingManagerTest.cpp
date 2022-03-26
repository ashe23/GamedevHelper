// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "IAssetTools.h"
#include "AssetRegistryModule.h"
#include "EditorAssetLibrary.h"
#include "Materials/Material.h"
#include "GamedevHelperAssetNamingManagerLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerTest,
	"Plugins.GamedevHelper.AssetNamingManager.AssetsRenaming",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

static const FString AutomationTestDirAbs = FPaths::ProjectContentDir() + TEXT("Automation/");
static const FString AutomationTestDirRel = TEXT("/Game/Automation/");

struct FTestRunner
{
	FTestRunner()
	{
		// create automation folder
		if (!PlatformFile.DirectoryExists(*AutomationTestDirAbs))
		{
			PlatformFile.CreateDirectoryTree(*AutomationTestDirAbs);
		}
	}
	~FTestRunner()
	{
		UEditorAssetLibrary::DeleteDirectory(AutomationTestDirRel);
		
		if (PlatformFile.DirectoryExists(*AutomationTestDirAbs))
		{
			if (!PlatformFile.DeleteDirectoryRecursively(*AutomationTestDirAbs))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to delete directory: %s"), *AutomationTestDirAbs);
			}
		}
	}
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
};

struct FAssetHelper
{
	template<typename AssetType>
	static FAssetData CreateAsset(const FString& AssetName, const UClass* AssetClass)
	{
		const FString AssetPath = AutomationTestDirAbs;
		const FString PackagePath = AutomationTestDirRel + AssetName;

		UPackage* Package = CreatePackage(*PackagePath);
		AssetType* MyDataAsset = NewObject<AssetType>(Package, AssetClass, *AssetName, RF_Public | RF_Standalone);

		FAssetRegistryModule::AssetCreated(MyDataAsset);
		MyDataAsset->MarkPackageDirty();

		const FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
		UPackage::SavePackage(Package, MyDataAsset, RF_Public | RF_Standalone, *FilePath);
		
		const FString ObjectPath = FString::Printf(TEXT("%s%s.%s"), *AutomationTestDirRel, *AssetName, *AssetName);
		const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
		
		return AssetRegistryModule.Get().GetAssetByObjectPath(FName{ObjectPath});
	}
};

struct FTestCase
{
	UClass* AssetClass;
	FString InputName;
	FString ExpectedName;
	FString ActualName;
};

bool FAssetNamingManagerTest::RunTest(const FString& Parameters)
{
	// const FTestRunner TestRunner;

	// const auto NamingConvention = GetDefault<UGamedevHelperAssetNamingConvention>();
	// if (!NamingConvention)
	// {
	// 	return false;
	// }
	//
	// TArray<FAssetData> Assets;
	// Assets.Add(FAssetHelper::CreateAsset<UMaterial>(TEXT("MyMaterial01"), UMaterial::StaticClass()));
	//
	// UGamedevHelperAssetNamingManagerLibrary::RenameAssets(Assets);

	// todo:ashe23	
	// TArray<FGamedevHelperRenamePreview> Previews;
	// UGamedevHelperAssetNamingManagerLibrary::GetRenamePreviews(Assets, NamingConvention, Previews);
	
	
	return true;
}
