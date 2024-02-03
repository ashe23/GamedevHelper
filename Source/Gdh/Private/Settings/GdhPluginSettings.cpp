// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhPluginSettings.h"

#include "Gdh.h"

UGdhPluginSettings::UGdhPluginSettings() {}

FName UGdhPluginSettings::GetContainerName() const
{
	return TEXT("Project");
}

FName UGdhPluginSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

FText UGdhPluginSettings::GetSectionText() const
{
	return FText::FromName(GdhConstants::ModuleFullName);
}
