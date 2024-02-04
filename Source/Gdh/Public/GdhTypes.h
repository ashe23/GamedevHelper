// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.generated.h"

DECLARE_MULTICAST_DELEGATE(FGdhSettingsChanged);

UENUM(BlueprintType)
enum class EGdhBlueprintType : uint8
{
	None,
	Normal,
	Interface,
	FunctionLibrary,
	MacroLibrary
};

UENUM(BlueprintType)
enum class EGdhRenameMethod : uint8
{
	None,
	FixPrefixAndSuffixOnly UMETA(ToolTip="Rename assets by fixing their prefixes and suffixes only according to data table asset"),
	SearchAndReplace
};

UENUM(BlueprintType)
enum class EGdhNamingCase : uint8
{
	None,
	PascalSnakeCase UMETA(DisplayName="Pascal_Snake_Case"),
	PascalCase UMETA(DisplayName="PascalCase"),
	UpperCase UMETA(DisplayName="UPPERCASE"),
	LowerCase UMETA(DisplayName="lowercase"),
	SnakeCase UMETA(DisplayName="snake_case"),
	CamelCase UMETA(DisplayName="camelCase"),
	KebabCase UMETA(DisplayName="kebab-case"),
};

USTRUCT(BlueprintType)
struct FGdhAssetNameFormatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh", meta=(ShowTreeView, AllowAbstract))
	TSoftClassPtr<UObject> AssetClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct FGdhAssetNameFormat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;
};

UCLASS(Transient)
class UGdhAssetNamingToolListItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FAssetData AssetData;

	UPROPERTY()
	FString OldName;

	UPROPERTY()
	FString NewName;
};

USTRUCT(BlueprintType)
struct FGdhAssetNamingInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gdh")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct FGdhAssetIndirectInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AssetIndirectInfo")
	FAssetData Asset;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AssetIndirectInfo")
	FString FilePath;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AssetIndirectInfo")
	int32 FileNum = 0;

	bool operator==(const FGdhAssetIndirectInfo& Other) const
	{
		return Asset == Other.Asset && FilePath.Equals(Other.FilePath) && FileNum == Other.FileNum;
	}

	bool operator!=(const FGdhAssetIndirectInfo& Other) const
	{
		return !(Asset == Other.Asset && FilePath.Equals(Other.FilePath) && FileNum == Other.FileNum);
	}
};
