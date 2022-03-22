// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.generated.h"

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return static_cast<EnumType>(Enum->GetIndexByName(FName(*String)));
}

UENUM(BlueprintType)
enum class EGamedevHelperNamingCase : uint8
{
	PascalCase UMETA(DisplayName = "PascalCase"),
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	KebabCase UMETA(DisplayName = "kebab-case"),
	CamelCase UMETA(DisplayName = "camelCase"),
};

UENUM(BlueprintType)
enum class EGamedevHelperVertexAnimTexture : uint8
{
	UV,
	Normal,
};

UENUM(BlueprintType)
enum class EGamedevHelperBlueprintType : uint8
{
	None,
	Normal,
	Component,
	Interface,
	MacroLibrary,
	FunctionLibrary
};

UENUM(BlueprintType)
enum class EGamedevHelperModalStatus : uint8
{
	None,
	Pending,
	Success,
	Fail,
};

UENUM(BlueprintType)
enum class EGamedevHelperAssetType : uint8
{
	Blueprint,
	EditorUtility,
	Material,
	Texture,
	FX,
	Animation,
	AI,
	Foliage,
	Sound,
	Slate,
	Paper2D,
	Misc,
	Custom
};

UENUM(BlueprintType)
enum class EGamedevHelperRenameStatus : uint8
{
	Ok,
	OkToRename,
	MissingSettings,
	DuplicateNameContentBrowser,
	DuplicateNamePreview
};

USTRUCT()
struct FGamedevHelperRenamePreview
{
	GENERATED_BODY()

	bool IsValid() const
	{
		return Status == EGamedevHelperRenameStatus::OkToRename;
	}

	FAssetData GetAssetData() const
	{
		return AssetData;
	}
	
	FString GetOldName() const
	{
		return AssetData.AssetName.ToString();
	}
	
	FString GetNewName() const
	{
		return NewName;
	}

	FString GetNewObjectPath() const
	{
		return NewObjectPath;
	}

	void SetStatus(const EGamedevHelperRenameStatus NewStatus)
	{
		Status = NewStatus;
	}

	void SetAssetData(const FAssetData Asset)
	{
		if (Asset.IsValid())
		{
			AssetData = Asset;
		}
	}

	void SetNewName(const FString& Name)
	{
		NewName = Name;
	}

	void SetNewObjectPath(const FString& Path)
	{
		NewObjectPath = Path;
	}

	EGamedevHelperRenameStatus GetStatus() const
	{
		return Status;
	}

	FString GetStatusMsg() const
	{
		if (Status == EGamedevHelperRenameStatus::MissingSettings)
		{
			return FString::Printf(TEXT("Missing namings for %s"), *AssetData.GetClass()->GetName());
		}

		if (Status == EGamedevHelperRenameStatus::DuplicateNamePreview)
		{
			return TEXT("Asset with same name already exists in previews");
		}

		if (Status == EGamedevHelperRenameStatus::DuplicateNameContentBrowser)
		{
			return TEXT("Asset with same name already exists at this location in content browser");
		}

		return TEXT("OK");
	}

private:
	UPROPERTY()
	FAssetData AssetData;

	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString NewObjectPath;

	UPROPERTY()
	FString ErrMsg;

	UPROPERTY()
	EGamedevHelperRenameStatus Status = EGamedevHelperRenameStatus::Ok;
};

USTRUCT(BlueprintType)
struct FGamedevHelperAssetNameFormat
{
	GENERATED_BODY()

	FGamedevHelperAssetNameFormat() = default;

	bool IsEmpty() const
	{
		return Prefix.IsEmpty() && Suffix.IsEmpty();
	}

	bool IsValid() const
	{
		return AssetClass && AssetClass->IsValidLowLevel() && IsEmpty() == false;
	}

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetNameFormat")
	UClass* AssetClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetNameFormat")
	FString Prefix;

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetNameFormat")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct FGamedevHelperAssetNamings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetNamingConvention")
	TArray<FGamedevHelperAssetNameFormat> Settings;
};
