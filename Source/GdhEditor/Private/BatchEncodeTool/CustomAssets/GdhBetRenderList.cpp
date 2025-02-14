// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"

#if WITH_EDITOR
void UGdhBetRenderList::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGdhBetRenderList, Sequences)) {
		OnRenderListChanged.Broadcast();
	}
}
#endif
