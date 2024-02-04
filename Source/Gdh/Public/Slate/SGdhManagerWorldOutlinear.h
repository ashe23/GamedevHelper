// // Copyright Ashot Barkhudaryan. All Rights Reserved.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Widgets/SCompoundWidget.h"
//
// class UGdhWorldOutlinearSettings;
//
// class SGdhManagerWorldOutlinear final : public SCompoundWidget
// {
// public:
// 	SLATE_BEGIN_ARGS(SGdhManagerWorldOutlinear) {}
// 	SLATE_END_ARGS()
//
// 	void Construct(const FArguments& InArgs);
//
// private:
// 	TSharedRef<SWidget> CreateToolbarMain() const;
// 	
// 	TSharedPtr<FUICommandList> Cmds;
// 	TWeakObjectPtr<UGdhWorldOutlinearSettings> WorldOutlinearSettings;
// };
