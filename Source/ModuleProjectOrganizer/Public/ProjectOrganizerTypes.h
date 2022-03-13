// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

// todo:ashe23 rename this enum later
UENUM(BlueprintType)
enum class EModuleProjectOrganizerNamingCase : uint8
{
	PascalCase UMETA(DisplayName = "PascalCase"),
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	KebabCase UMETA(DisplayName = "kebab-case"),
	CamelCase UMETA(DisplayName = "camelCase"),
};
