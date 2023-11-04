// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "GdhTypes.h"
#include "EditorSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
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
	 * @param Assets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static void GetAssetsAll(TArray<FAssetData>& Assets);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FString GetAssetRenamePreview(const FAssetData& InAssetData);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FGdhAssetNamingInfo GetAssetNamingInfoByAsset(const FAssetData& InAssetData);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Lib_Asset")
	static FGdhAssetNamingInfo GetAssetNamingInfoByAssetClass(const UClass* InAssetClass);

	/**
	 * @brief Return blueprint type
	 * @param InAssetData FAssetData
	 * @return EGdhBlueprintType
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static EGdhBlueprintType GetBlueprintType(const FAssetData& InAssetData);


	/**
	 * @brief Returns blueprint asset parent class
	 * @param InAssetData FAssetData
	 * @return UClass*
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static UClass* GetBlueprintParentClass(const FAssetData& InAssetData);

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

	static FAssetRegistryModule& GetModuleAssetRegistry();
	static FPropertyEditorModule& GetModulePropertyEditor();
	static FAssetToolsModule& GetModuleAssetTools();

private:
	/**
	 * @brief Removes old prefixes and suffixes from old asset name. Asset name must be normalized.
	 * @param OldAssetName FString
	 * @param NamingConvention UGamedevHelperAssetNamingConvention
	 * @return FString
	 */
	static FString RemoveOldPrefixAndSuffix(const FString& OldAssetName, const UGdhAssetNamingConvention* NamingConvention);
};
