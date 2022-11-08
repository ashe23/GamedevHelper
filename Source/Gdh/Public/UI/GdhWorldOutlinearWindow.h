// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/GdhWorldOutlinearSettings.h"
#include "Widgets/SCompoundWidget.h"

class SGdhWorldOutlinearWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhWorldOutlinearWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	FReply OnOrganizeBtnClicked() const;
	static FReply OnUndoBtnClicked();
private:
	FName GetFolderNameByActor(const AActor* Actor) const;
	
	TWeakObjectPtr<UGdhWorldOutlinearSettings> WorldOutlinearSettings;
};