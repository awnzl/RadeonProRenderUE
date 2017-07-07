// RPR COPYRIGHT

#pragma once

#include "CoreMinimal.h"
#include "RadeonProRender.h"
#include "GameFramework/Actor.h"
#include "RPRScene.generated.h"

/**
* Handles the context and scene creation
*/
UCLASS()
class ARPRScene : public AActor
{
	GENERATED_BODY()
public:
	ARPRScene();

	// TMP
	UPROPERTY(EditAnywhere)
	UTexture2D	*BackgroundImage;


private:
	virtual void	BeginDestroy() override;
	virtual void	BeginPlay() override;
	virtual void	Tick(float deltaTime) override;

	void	BuildRPRActor(UWorld *world, USceneComponent *srcComponent, UClass *typeClass);
	void	BuildScene();
public:
	rpr_context		m_RprContext;
	rpr_scene		m_RprScene;
	rpr_framebuffer	m_RprFrameBuffer;

	//// TMP
	rpr_image		m_RprBackgroundImage;
private:
	class FRPRRendererWorker	*m_RendererWorker;

	TSharedPtr<UTexture2DDynamic>	RenderTexture;

	UPROPERTY(Transient)
	TArray<AActor*>		SceneContent;
};
