// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibPath.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhPathLibrary"))
class GDHUTIL_API UGdhLibPath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Normalize given path
	 * @param InPath FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Path")
	static FString Normalize(const FString& InPath);

	/**
	 * @brief Convert given path to absolute
	 * @param InPath FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static FString PathConvertToAbsolute(const FString& InPath);

	/**
	 * @brief Convert given path relative to Content folder
	 * @param InPath FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static FString PathConvertToRelative(const FString& InPath);

	/**
	 * @brief Convert given path to object path 
	 * @param InPath FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static FString PathConvertToObjectPath(const FString& InPath);

	/**
	 * @brief Returns list of files inside given path with specified settings
	 * @param InSearchPath FString
	 * @param bSearchRecursive bool
	 * @param OutFiles
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static void GetFiles(const FString& InSearchPath, const bool bSearchRecursive, TArray<FString>& OutFiles);

	/**
	 * @brief Returns list of files inside given path with specified file extension settings
	 * @param InSearchPath FString
	 * @param bSearchRecursive bool
	 * @param bExtSearchInvert bool
	 * @param InExtensions TSet<FString>
	 * @param OutFiles
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static void GetFilesByExt(const FString& InSearchPath, const bool bSearchRecursive, const bool bExtSearchInvert, const TSet<FString>& InExtensions, TArray<FString>& OutFiles);

	/**
	 * @brief Returns all subfolders in given path
	 * @param InSearchPath FString
	 * @param bSearchRecursive bool
	 * @param OutFolders
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static void GetFolders(const FString& InSearchPath, const bool bSearchRecursive, TArray<FString>& OutFolders);

	/**
	 * @brief Returns size of given file in bytes
	 * @param InFile FString
	 * @return int64
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static int64 GetFileSize(const FString& InFile);

	/**
	 * @brief Returns total size of given files in bytes
	 * @param Files TArray<FString>
	 * @return int64
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectCleanerSubsystem|Lib_Path")
	static int64 GetFilesTotalSize(const TArray<FString>& Files);
};
