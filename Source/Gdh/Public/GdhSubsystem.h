// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "GdhTypes.h"
#include "EditorSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GdhSubsystem.generated.h"

class UGdhAssetNamingConvention;

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="GamedevHelperSubsystem")
class UGdhSubsystem final : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/**
	 * @brief Returns all assets in project (particularly Content folder)
	 * @param OutAssets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void GetAssetsAll(TArray<FAssetData>& OutAssets);

	/**
	 * @brief Returns all asset in given path
	 * @param InPath FString
	 * @param bRecursive bool
	 * @param OutAssets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void GetAssetsByPath(const FString& InPath, const bool bRecursive, TArray<FAssetData>& OutAssets);

	/**
	 * @brief Returns asset by given object path
	 * @param InPath FString
	 * @return FAssetData
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FAssetData GetAssetByObjectPath(const FString& InPath);

	/**
	 * @brief Returns all redirectors in project
	 * @param Redirectors TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void GetProjectRedirectors(TArray<FAssetData>& Redirectors);

	/**
	 * @brief Checks if project contains any redirector asset
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static bool ProjectHasRedirectors();

	/**
	 * @brief Fixup given redirector assets
	 * @param Redirectors TArray<FAssetData>
	 * @param bShowSlowTask bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void FixProjectRedirectors(const TArray<FAssetData>& Redirectors, const bool bShowSlowTask = true);

	/**
	 * @brief Returns new asset name preview based on asset naming convention settings
	 * @param InAssetData FAssetData
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FString GetAssetRenamePreview(const FAssetData& InAssetData);

	/**
	 * @brief Returns asset naming info by given asset data
	 * @param InAssetData FAssetData
	 * @return FGdhAssetNamingInfo
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FGdhAssetNamingInfo GetAssetNamingInfoByAsset(const FAssetData& InAssetData);

	/**
	 * @brief Return asset naming info by given asset class
	 * @param InAssetClass UClass*
	 * @return FGdhAssetNamingInfo
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FGdhAssetNamingInfo GetAssetNamingInfoByAssetClass(const UClass* InAssetClass);

	/**
	 * @brief Return blueprint type
	 * @param InAssetData FAssetData
	 * @return EGdhBlueprintType
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static EGdhBlueprintType GetBlueprintType(const FAssetData& InAssetData);


	/**
	 * @brief Returns blueprint asset parent class
	 * @param InAssetData FAssetData
	 * @return UClass*
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static UClass* GetBlueprintParentClass(const FAssetData& InAssetData);

	/**
	 * @brief Opens editor for given asset
	 * @param InAssetData FAssetData
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void OpenAssetEditor(const FAssetData& InAssetData);

	/**
	 * @brief Opens given asset in content browser
	 * @param InAssetData FAssetData
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void OpenAssetInContentBrowser(const FAssetData& InAssetData);

	/**
	 * @brief Returns normalized string by removing all extra underscores and hyphens from string start and end, then replaces by underscore in the middle of string 
	 * - _aa_ => aa
	 * - aaa => aaa
	 * - aaa___bbb___ccc => aaa_bbb_ccc
	 * - -aa-bb-cc--01- => aa_bb_cc_01
	 * 
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_String")
	static FString Normalize(const FString& OriginalString);

	/**
	 * @brief Returns new string with contextual parts splitted by underscore. String can be in any naming case, even mixed.
	 * - PascalCase01 => pascal_case_01
	 * - 01_my-string => 01_my_string
	 * - TextureUV01 => texture_uv_01
	 * @param OriginalString FString
	 * @return FString Result is always in snake_case
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_String")
	static FString Tokenize(const FString& OriginalString);

	/**
	 * @brief Converts string to given naming case
	 * @param OriginalString FString
	 * @param NamingCase EGamedevHelperNamingCase
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_String")
	static FString ConvertNamingCase(const FString& OriginalString, const EGdhNamingCase NamingCase);

	/**
	 * @brief Converts string to PascalCase
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_String")
	static FString ConvertToPascalCase(const FString& OriginalString);

	/**
	 * @brief Converts string to Pascal_Snake_Case
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_String")
	static FString ConvertToPascalSnakeCase(const FString& OriginalString);

	/**
	 * @brief Converts string to snake_case
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_String")
	static FString ConvertToSnakeCase(const FString& OriginalString);

	static void ShowNotification(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration);
	static void ShowNotificationWithOutputLog(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration);

	static FAssetRegistryModule& GetModuleAssetRegistry();
	static FPropertyEditorModule& GetModulePropertyEditor();
	static FAssetToolsModule& GetModuleAssetTools();
	static FContentBrowserModule& GetModuleContentBrowser();

private:
	/**
	 * @brief Removes old prefixes and suffixes from old asset name. Asset name must be normalized.
	 * @param OldAssetName FString
	 * @param NamingConvention UGamedevHelperAssetNamingConvention
	 * @return FString
	 */
	static FString RemoveOldPrefixAndSuffix(const FString& OldAssetName, const UGdhAssetNamingConvention* NamingConvention);
};
