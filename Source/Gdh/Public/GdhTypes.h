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
enum class EGdhNamingCase : uint8
{
	None,
	SnakeCase UMETA(DisplayName="snake_case"),
	PascalCase UMETA(DisplayName="PascalCase"),
	PascalSnakeCase UMETA(DisplayName="Pascal_Snake_Case")
};

UCLASS(Transient)
class UGdhManagerAssetNamingItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FAssetData AssetData;

	UPROPERTY()
	FString OldName;

	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString Note;

	UPROPERTY()
	FLinearColor NoteColor = FLinearColor::White;
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
