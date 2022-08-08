// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/Renderer/GamedevHelperRendererQueueItem.h"

// #include "Renderer/GamedevHelperRendererQueueItem.h"

// UVirtualGamesRendererQueueItem::UVirtualGamesRendererQueueItem()
// {
// 	Status = EVirtualGamesStatus::None;
// 	PipelineName = TEXT("");
// 	ShotName = TEXT("");
// 	ShotDuration = TEXT("");
// 	ShotDurationInFrames = 0;
// 	ShotStartFrame = 0;
// 	ShotRenderedFrames = 0;
// 	ShotOutputDir = TEXT("");
// 	Note = TEXT("");
// }
//
// bool UVirtualGamesRendererQueueItem::IsRendered() const
// {
// 	const UVirtualGamesRendererSettings* RendererSettings = GetDefault<UVirtualGamesRendererSettings>();
// 	if (!RendererSettings) return false;
// 	if (!RendererSettings->IsValid()) return false;
//
// 	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
// 	// const double Framerate = RendererSettings->Framerate.AsDecimal();
// 	const FString Resolution = RendererSettings->GetResolutionAsString(TEXT("_"));
// 	const FString ImageFormat = RendererSettings->GetImageFormatAsString();
//
// 	struct DirectoryVisitor : IPlatformFile::FDirectoryVisitor
// 	{
// 		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
// 		{
// 			if (bIsDirectory) return false;
// 	
// 			const FString BaseName = FPaths::GetBaseFilename(FilenameOrDirectory);
// 			const FString Extension = FPaths::GetExtension(FilenameOrDirectory);
// 			if (BaseName.StartsWith(RequiredBaseName) && Extension.Equals(ImageFormat))
// 			{
// 				TArray<FString> Parts;
// 				BaseName.ParseIntoArray(Parts, TEXT("_"));
// 				Frames.Add(FCString::Atoi(*Parts.Last()));
// 			}
// 			
// 			return true;
// 		}
// 	
// 		FString RequiredBaseName;
// 		FString ImageFormat;
// 		TArray<int32> Frames;
// 	};
//
// 	const FString RenderedImageFileName = FString::Printf(
// 		TEXT("%s_%s"),
// 		*ShotName,
// 		*Resolution
// 	);
// 	
// 	const FString RenderedImagePath = FString::Printf(
// 		TEXT("%s/image/%s/"),
// 		*RendererSettings->OutputDir.Path,
// 		*PipelineName
// 	);
//
// 	DirectoryVisitor Visitor;
// 	Visitor.RequiredBaseName = RenderedImageFileName;
// 	Visitor.ImageFormat = ImageFormat;
// 	if (!PlatformFile.IterateDirectory(*RenderedImagePath, Visitor))
// 	{
// 		return false;
// 	}
//
// 	if (Visitor.Frames.Num() == 0)
// 	{
// 		return false;
// 	}
//
// 	Visitor.Frames.Sort();
// 	for (int32 i = 0; i < Visitor.Frames.Num(); ++i)
// 	{
// 		if (Visitor.Frames[i] != i)
// 		{
// 			return false;
// 		}
// 	}
//
// 	return true;
// }
//
// bool UVirtualGamesRendererQueueItem::IsValid() const
// {
// 	return Status != EVirtualGamesStatus::Error;
// }
//
// int32 UVirtualGamesRendererQueueItem::GetRenderedFramesNum() const
// {
// 	const UVirtualGamesRendererSettings* RendererSettings = GetDefault<UVirtualGamesRendererSettings>();
// 	if (!RendererSettings) return false;
// 	if (!RendererSettings->IsValid()) return false;
//
// 	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
// 	const FString Resolution = RendererSettings->GetResolutionAsString(TEXT("_"));
// 	const FString ImageFormat = RendererSettings->GetImageFormatAsString();
//
// 	struct DirectoryVisitor : IPlatformFile::FDirectoryVisitor
// 	{
// 		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
// 		{
// 			if (bIsDirectory) return false;
// 	
// 			const FString BaseName = FPaths::GetBaseFilename(FilenameOrDirectory);
// 			const FString Extension = FPaths::GetExtension(FilenameOrDirectory);
// 			if (BaseName.StartsWith(RequiredBaseName) && Extension.Equals(ImageFormat))
// 			{
// 				RenderedFrames++;
// 			}
// 			
// 			return true;
// 		}
// 	
// 		FString RequiredBaseName;
// 		FString ImageFormat;
// 		int32 RenderedFrames = 0;
// 	};
//
// 	const FString RenderedImageFileName = FString::Printf(
// 		TEXT("%s_%s"),
// 		*ShotName,
// 		*Resolution
// 	);
// 	
// 	const FString RenderedImagePath = FString::Printf(
// 		TEXT("%s/image/%s/"),
// 		*RendererSettings->OutputDir.Path,
// 		*PipelineName
// 	);
//
// 	DirectoryVisitor Visitor;
// 	Visitor.RequiredBaseName = RenderedImageFileName;
// 	Visitor.ImageFormat = ImageFormat;
//
// 	if (PlatformFile.IterateDirectory(*RenderedImagePath, Visitor))
// 	{
// 		return Visitor.RenderedFrames;
// 	}
// 	
// 	return 0; 
// }
UGamedevHelperRendererQueueItem::UGamedevHelperRendererQueueItem()
{
	Status = EGamedevHelperRendererStatus::None;
	PipelineName = TEXT("");
	SequenceName = TEXT("");
}
