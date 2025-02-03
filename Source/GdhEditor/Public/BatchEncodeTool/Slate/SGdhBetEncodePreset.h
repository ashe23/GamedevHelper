// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhBetEncodePreset;

class SGdhBetEncodePreset final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhBetEncodePreset) {}
	SLATE_ARGUMENT(UGdhBetEncodePreset*, EncodePreset)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	UGdhBetEncodePreset* EncodePreset = nullptr;
};
