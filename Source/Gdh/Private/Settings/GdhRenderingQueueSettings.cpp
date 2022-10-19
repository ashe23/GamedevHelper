// // Copyright Ashot Barkhudaryan. All Rights Reserved.
//
// #include "Settings/GdhRenderingQueueSettings.h"
// // Engine Headers
//
// #if WITH_EDITOR
// void UGdhRenderingQueueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	SaveConfig();
//
// 	if (GdhQueueSettingsOnChangeDelegate.IsBound())
// 	{
// 		GdhQueueSettingsOnChangeDelegate.Execute();
// 	}
// }
// #endif
