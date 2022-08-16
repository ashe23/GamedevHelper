// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperSubsystem.generated.h"

UCLASS()
class UGamedevHelperSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// VERTEX ANIMATION TOOLS

	/**
	 * @brief Configures static mesh for vertex animation (LODs included)
	 * @see https://docs.unrealengine.com/4.27/en-US/AnimatingObjects/SkeletalMeshAnimation/Tools/VertexAnimationTool/
	 * @param StaticMesh UStaticMesh*
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|VertexAnimTools")
	static bool ConfigureStaticMeshForVertexAnim(UStaticMesh* StaticMesh);

	/**
	 * @brief Configures array of static meshes for vertex animation (LODs included)
	 * @see https://docs.unrealengine.com/4.27/en-US/AnimatingObjects/SkeletalMeshAnimation/Tools/VertexAnimationTool/
	 * @param StaticMeshes TArray<UStaticMesh*>
	 * @param bShowNotification bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|VertexAnimTools")
	static void ConfigureStaticMeshesForVertexAnim(TArray<UStaticMesh*> StaticMeshes, const bool bShowNotification = true);

	/**
	 * @brief Configures texture of specified type for vertex animation
	 * @see https://docs.unrealengine.com/4.27/en-US/AnimatingObjects/SkeletalMeshAnimation/Tools/VertexAnimationTool/
	 * @param Texture UTexture2D*
	 * @param TextureType EGamedevHelperVertexAnimTexture
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|VertexAnimTools")
	static bool ConfigureTextureForVertexAnim(UTexture2D* Texture, const EGamedevHelperVertexAnimTexture TextureType);

	/**
	 * @brief Configures textures of specified type for vertex animation
	 * @see https://docs.unrealengine.com/4.27/en-US/AnimatingObjects/SkeletalMeshAnimation/Tools/VertexAnimationTool/
	 * @param Textures UTexture2D*
	 * @param TextureType EGamedevHelperVertexAnimTexture
	 * @param bShowNotification bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|VertexAnimTools")
	static void ConfigureTexturesForVertexAnim(TArray<UTexture2D*> Textures, const EGamedevHelperVertexAnimTexture TextureType, const bool bShowNotification = true);

	// ASSET LIBRARY FUNCTIONS

	/**
	 * @brief Returns selected assets in content browser
	 * @param SelectedAssets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetLibrary")
	static void GetSelectedAssets(TArray<FAssetData>& SelectedAssets);

	/**
	 * @brief Disable collision on given static meshes
	 * @param StaticMesh UStaticMesh*
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetLibrary")
	static bool DisableCollision(UStaticMesh* StaticMesh);

	/**
	 * @brief Disable collisions on array of static meshes
	 * @param StaticMeshes TArray<UStaticMesh*>
	 * @param bShowNotification bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetLibrary")
	static void DisableCollisions(TArray<UStaticMesh*> StaticMeshes, const bool bShowNotification = true);

	// NOTIFICATION LIBRARY FUNCTIONS

	/**
	 * @brief Shows notification modal window
	 * @param Msg Notification Text
	 * @param ModalStatus Notification Status
	 * @param Duration Notification Duration
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|NotificationLibrary")
	static void ShowModal(const FString& Msg, const EGamedevHelperModalStatus ModalStatus, const float Duration = 2.0f);

	/**
	 * @brief Shows notification modal window with link to OutputLog
	 * @param Msg Notification Text
	 * @param Duration Notification Duration
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|NotificationLibrary")
	static void ShowModalWithOutputLog(const FString& Msg, const float Duration = 2.0f);

	/**
	 * @brief  Shows notification modal window with given link
	 * @param Msg Notification Text
	 * @param Link Notification Link
	 * @param Duration Notification Duration
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|NotificationLibrary")
	static void ShowModalWithHyperLink(const FString& Msg, const FString& Link, const float Duration = 2.0f);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|RendererLibrary")
	static void RenderMovieRenderQueue(const TArray<TSoftObjectPtr<UMoviePipelineQueue>> QueueAssets);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|RendererLibrary")
	static void GetLevelSequencePlaybackInfo(const TSoftObjectPtr<ULevelSequence> LevelSequence, FGamedevHelperSequencePlaybackInfo& PlaybackInfo);
	
	static void RunFFmpegPythonScript(const TArray<FGamedevHelperFFmpegCommand>& FFmpegCommands);

	template <class ClassType>
	static void GetSelectedAssets(TArray<ClassType*>& Assets);
private:
	void RegisterContextMenuActions() const;
	
	static SNotificationItem::ECompletionState GetCompletionStateFromModalStatus(const EGamedevHelperModalStatus ModalStatus);
};




