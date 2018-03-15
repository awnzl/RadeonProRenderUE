#pragma once

#include "IUVProjectionAlgorithm.h"
#include "PositionVertexBuffer.h"
#include "StaticMeshVertexBuffer.h"
#include "ColorVertexBuffer.h"
#include "GCObject.h"
#include "SharedPointer.h"
#include "RawMesh.h"

/*
 * Abstraction class for UV projection algorithms
 */
class RPRPLUGINEDITOR_API FUVProjectionAlgorithmBase : public IUVProjectionAlgorithm
{
protected:

	typedef TArray<FVector2D>	FUVPack;

public:

	static bool	AreStaticMeshRenderDatasValid(UStaticMesh* InStaticMesh);

	virtual ~FUVProjectionAlgorithmBase() {}
	
	virtual void SetStaticMeshes(const TArray<class UStaticMesh*>& StaticMeshes) override;
	virtual void SetGlobalUVProjectionSettings(FUVProjectionSettingsPtr Settings);
	virtual FOnAlgorithmCompleted& OnAlgorithmCompleted() override;

	virtual void StartAlgorithm() override;
	virtual void AbortAlgorithm() override;
	virtual bool IsAlgorithimRunning() override;


protected:

	/* Clear the UV array and reserve space to fill it, based on the number of RawMesh.WedgeIndices. */
	void	PrepareUVs();

	void	StopAlgorithm();
	void	RaiseAlgorithmCompletion(bool bIsSuccess);
	void	StopAlgorithmAndRaiseCompletion(bool bIsSuccess);
	bool	AreStaticMeshRenderDatasValid() const;
	void	ApplyUVsOnMesh();
	void	SaveRawMesh();
	void	AddNewUVs(int32 RawMeshIndex, const FVector2D& UV);
	void	FixInvalidUVsHorizontally(int32 MeshIndex);


private:

	void	LoadRawMeshes();
	void	FixTextureCoordinateOnLeftSideIfRequired(float& TextureCoordinate);


protected:

	TArray<class UStaticMesh*>	StaticMeshes;
	TArray<FRawMesh>			RawMeshes;

	FOnAlgorithmCompleted		OnAlgorithmCompletedEvent;
	FUVProjectionSettingsPtr	UVProjectionSettings;

private:

	bool bIsAlgorithmRunning;
	TArray<FUVPack>	NewUVs;

};