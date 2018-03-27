#include "UVViewportClient.h"
#include "AssetEditorModeManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "RawMesh.h"
#include "UVUtility.h"
#include "SUVViewport.h"
#include "Engine/Selection.h"
#include "UVViewportActions.h"
#include "UVCacheData.h"
#include "RPRConstAway.h"

#define LOCTEXT_NAMESPACE "UVViewportClient"

DECLARE_LOG_CATEGORY_CLASS(UVViewportClientLog, Log, All)

struct HUVVertexProxy : public HHitProxy
{
	DECLARE_HIT_PROXY()

public:

	HUVVertexProxy(UUVCacheData* InUV)
		: UV(InUV)
	{}

	UUVCacheData* UV;

};

IMPLEMENT_HIT_PROXY(HUVVertexProxy, HHitProxy)

const FEditorModeID FUVViewportClient::UVModeID("EM_UV");

FUVViewportClient::FUVViewportClient(const TWeakPtr<SEditorViewport>& InViewport)
	: FEditorViewportClient(nullptr, nullptr, InViewport)
	, VertexColor(FLinearColor::White)
	, SelectedVertexColor(FLinearColor(0.98f, 0.47f, 0.0f)) // orange
	, ValidEdgeColor(FLinearColor::White)
	, InvalidEdgeColor(FLinearColor::Red)
{
	bDrawAxes = false;

	PreviewScene = &OwnedPreviewScene;
	bIsManipulating = false;

	// Scale the UV drawn so it is better to navigate in the scene
	SceneTransform.SetScale3D(FVector::OneVector * 100);

	SetRealtime(true);
	EngineShowFlags.DisableAdvancedFeatures();

	SetupCamera();
	GenerateCacheUV();
}

FUVViewportClient::~FUVViewportClient()
{
	UVCache.ClearCache();
}

void FUVViewportClient::GenerateCacheUV()
{
	DeselectAll();
	bIsManipulating = false;

	SUVViewportPtr viewport = GetUVViewport();
	if (viewport.IsValid())
	{
		TWeakPtr<FRPRMeshData> meshData = viewport->GetRPRMeshData();
		if (meshData.IsValid())
		{
			const FRawMesh& rawMesh = meshData.Pin()->GetRawMesh();
			UVCache.GenerateCache(rawMesh, viewport->GetUVChannel());
		}
	}
	else
	{
		UVCache.ClearCache();
	}
}

void FUVViewportClient::SetupCamera()
{
	SetViewportType(LVT_OrthoXZ);
	SetOrthoZoom(2500.0f);

	const FVector cameraLocation(50, 1, 50);
	const FRotator cameraRotation = UKismetMathLibrary::FindLookAtRotation(cameraLocation, FVector::ZeroVector);
	SetCameraSetup(FVector::ZeroVector, cameraRotation, FVector::ZeroVector, FVector::ZeroVector, cameraLocation, cameraRotation);
}

void FUVViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);

	SUVViewportPtr viewport = GetUVViewport();
	if (viewport.IsValid())
	{
		DrawUV(View, PDI, viewport);
	}
}

void FUVViewportClient::DrawUV(const FSceneView* View, FPrimitiveDrawInterface* PDI, SUVViewportPtr UVViewport)
{
	FRPRMeshDataPtr meshData = UVViewport->GetRPRMeshData().Pin();
	const FRawMesh& rawMesh = meshData->GetRawMesh();
	const int32 uvChannel = UVViewport->GetUVChannel();

	const TArray<FVector2D>& uv = rawMesh.WedgeTexCoords[uvChannel];
	const TArray<uint32>& triangles = rawMesh.WedgeIndices;
	
	for (int32 triIdx = 0; triIdx < triangles.Num(); triIdx += 3)
	{
		if (!uv.IsValidIndex(triIdx))
		{
			break;
		}

		const FVector2D& uvA = uv[triIdx];
		const FVector2D& uvB = uv[triIdx + 1];
		const FVector2D& uvC = uv[triIdx + 2];

		if (FUVUtility::IsUVTriangleValid(uvA, uvB, uvC))
		{
			DrawUVTriangle(PDI, triIdx, ValidEdgeColor, uvA, uvB, uvC);
		}
		else
		{
			DrawUVTriangle(PDI, triIdx, InvalidEdgeColor, uvA, uvB, uvC);
		}
	}
}

void FUVViewportClient::DrawUVTriangle(FPrimitiveDrawInterface* PDI, int32 UVStartIndex, const FLinearColor& Color,
									const FVector2D& uvA, const FVector2D& uvB, const FVector2D& uvC)
{
	FVector uvA_3D = ConvertUVto3D(uvA);
	FVector uvB_3D = ConvertUVto3D(uvB);
	FVector uvC_3D = ConvertUVto3D(uvC);

	const uint8 depthPriority = SDPG_World;
	PDI->DrawLine(uvA_3D, uvB_3D, Color, depthPriority);
	PDI->DrawLine(uvB_3D, uvC_3D, Color, depthPriority);
	PDI->DrawLine(uvC_3D, uvA_3D, Color, depthPriority);

	DrawUVVertex(PDI, UVStartIndex, uvA_3D);
	DrawUVVertex(PDI, UVStartIndex + 1, uvB_3D);
	DrawUVVertex(PDI, UVStartIndex + 2, uvC_3D);
}

void FUVViewportClient::DrawUVVertex(FPrimitiveDrawInterface* PDI, int32 UVIndex, const FVector& UV_3D)
{
	const int32 vertexSize = 5.0f;
	const uint8 depthPriority = SDPG_World;

	UUVCacheData* uv = UVCache[UVIndex];
	PDI->SetHitProxy(new HUVVertexProxy(uv));
	FLinearColor color = VertexColor;
	
	if (ModeTools->GetSelectedObjects()->IsSelected(uv))
	{
		color = SelectedVertexColor;
	}

	PDI->DrawPoint(UV_3D, color, vertexSize, depthPriority);
	PDI->SetHitProxy(nullptr);
}

void FUVViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	FEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);

	if (HitProxy != nullptr && HitProxy->IsA(HUVVertexProxy::StaticGetType()))
	{
		HUVVertexProxy* uvProxy = StaticCast<HUVVertexProxy*>(HitProxy);
		ModeTools->GetSelectedObjects()->Select(uvProxy->UV);
	}
	else
	{
		ModeTools->GetSelectedObjects()->DeselectAll();
	}
}

void FUVViewportClient::TrackingStarted(const struct FInputEventState& InInputState, bool bIsDragging, bool bNudge)
{
	if (!bIsManipulating && bIsDragging)
	{
		if (HasUVSelected())
		{
			FText transText;
			if (GetWidgetMode() == FWidget::WM_Translate)
			{
				transText = LOCTEXT("FUVViewportClient_TranslateUV", "Translate UV");
			}
			else if (GetWidgetMode() == FWidget::WM_Rotate)
			{
				transText = LOCTEXT("FUVViewportClient_RotateUV", "Rotate UV");
			}
			else
			{
				transText = LOCTEXT("FUVViewportClient_ScaleUV", "Scale UV");

				TArray<FVector2D> uv;
				GetSelectedUV(uv);
				ScaleModifierContext.SetupContext(uv, GetUVSelectionBarycenter(), SceneTransform);
			}

			GEditor->BeginTransaction(transText);
		}

		bIsManipulating = true;
	}
}

void FUVViewportClient::TrackingStopped()
{
	if (bIsManipulating)
	{
		bIsManipulating = false;
		GEditor->EndTransaction();

		if (HasUVSelected())
		{
			SUVViewportPtr viewport = GetUVViewport();
			if (viewport.IsValid())
			{
				TWeakPtr<FRPRMeshData> meshData = viewport->GetRPRMeshData();
				meshData.Pin()->ApplyRawMeshDatas();
			}
		}
	}
}

bool FUVViewportClient::InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale)
{
	bool bHandled = false;

	if (bIsManipulating)
	{
		if (CurrentAxis != EAxisList::None)
		{
			UProperty* ChangedProperty = NULL;
			const FWidget::EWidgetMode MoveMode = GetWidgetMode();
			switch (MoveMode)
			{
			case FWidget::WM_Translate:
				ApplyTranslation(Drag);
				break;

			case FWidget::WM_Rotate:
				ApplyRotation(Rot);
				break;

			case FWidget::WM_Scale:
				ApplyScale(Scale);
				break;

			default:
				break;
			}

			bHandled = true;
		}
	}

	return (bHandled);
}

bool FUVViewportClient::ShouldOrbitCamera() const
{
	return (false);
}

void FUVViewportClient::RegenerateUVCache()
{
	// If the UV cache is trying to be regenerate while we are manipulating,
	// it means that it is the viewport client that is doing the changes
	if (!bIsManipulating)
	{
		GenerateCacheUV();
	}
}

void FUVViewportClient::SelectAllUVs()
{
	USelection* selection = ModeTools->GetSelectedObjects();
	selection->BeginBatchSelectOperation();
	{
		for (int32 i = 0; i < UVCache.Num(); ++i)
		{
			selection->Select(UVCache[i]);
		}
	}
	selection->EndBatchSelectOperation();
}

FWidget::EWidgetMode FUVViewportClient::GetWidgetMode() const
{
	return (ModeTools->GetSelectedObjects()->Num() > 0 ? ModeTools->GetWidgetMode() : FWidget::EWidgetMode::WM_None);
}

FVector FUVViewportClient::GetWidgetLocation() const
{
	USelection* selection = ModeTools->GetSelectedObjects();
	if (selection->Num() > 0)
	{
		const FWidget::EWidgetMode MoveMode = GetWidgetMode();
		
		SUVViewportPtr viewport = GetUVViewport();
		if (viewport.IsValid())
		{
			const FVector2D barycenter = GetUVSelectionBarycenter();
			return (ConvertUVto3D(barycenter));
		}
	}

	return (FVector::ZeroVector);
}

SUVViewportPtr FUVViewportClient::GetUVViewport() const
{
	TSharedPtr<SEditorViewport> viewport = EditorViewportWidget.Pin();
	return (StaticCastSharedPtr<SUVViewport>(viewport));
}

FVector FUVViewportClient::ConvertUVto3D(const FVector2D& UV) const
{
	return (SceneTransform.TransformPosition(FVector(UV.X, 0, UV.Y)));
}

FVector2D FUVViewportClient::Convert3DtoUV(const FVector& In3D) const
{
	FVector transformed = SceneTransform.InverseTransformPosition(In3D);
	return (FVector2D(transformed.X, transformed.Z));
}

FVector2D FUVViewportClient::GetUVSelectionBarycenter() const
{
	USelection* selection = ModeTools->GetSelectedObjects();
	TArray<UUVCacheData*> uvCacheData;
	selection->GetSelectedObjects<UUVCacheData>(uvCacheData);

	FVector2D barycenter = FVector2D::ZeroVector;

	if (uvCacheData.Num() > 0)
	{
		SUVViewportPtr viewport = GetUVViewport();
		if (viewport.IsValid())
		{
			const TArray<FVector2D>& uv = viewport->GetUV();
			for (int32 i = 0; i < uvCacheData.Num(); ++i)
			{
				barycenter += uv[uvCacheData[i]->UVIndex];
			}

			barycenter /= uvCacheData.Num();
		}
	}

	return (barycenter);
}

bool FUVViewportClient::HasUVSelected() const
{
	return (ModeTools->GetSelectedObjects()->Num() > 0);
}

bool FUVViewportClient::GetSelectedUV(TArray<UUVCacheData*>& OutUVCacheData) const
{
	USelection* selection = ModeTools->GetSelectedObjects();
	if (selection == nullptr)
	{
		return (false);
	}

	selection->GetSelectedObjects<UUVCacheData>(OutUVCacheData);
	return (OutUVCacheData.Num() > 0);
}

bool FUVViewportClient::GetSelectedUV(TArray<FVector2D>& OutUV) const
{
	TArray<UUVCacheData*> uvCacheData;
	if (!GetSelectedUV(uvCacheData))
	{
		return (false);
	}

	SUVViewportPtr viewport = GetUVViewport();
	if (!viewport.IsValid())
	{
		return (false);
	}

	TWeakPtr<FRPRMeshData> meshData = viewport->GetRPRMeshData();
	if (!meshData.IsValid())
	{
		return (false);
	}

	const FRawMesh& rawMesh = meshData.Pin()->GetRawMesh();
	const TArray<FVector2D>& uv = rawMesh.WedgeTexCoords[viewport->GetUVChannel()];

	OutUV.Empty(uvCacheData.Num());
	for (int32 i = 0; i < uvCacheData.Num(); ++i)
	{
		OutUV.Add(uv[uvCacheData[i]->UVIndex]);
	}

	return (true);
}

TArray<FVector2D>& FUVViewportClient::GetRawMeshUV()
{
	const FUVViewportClient* thisConst = this;
	return (RPR::ConstRefAway(thisConst->GetRawMeshUV()));
}

const TArray<FVector2D>& FUVViewportClient::GetRawMeshUV() const
{
	SUVViewportPtr viewport = GetUVViewport();
	check(viewport.IsValid());

	TWeakPtr<FRPRMeshData> meshData = viewport->GetRPRMeshData();
	check(meshData.IsValid());

	const FRawMesh& rawMesh = meshData.Pin()->GetRawMesh();
	const TArray<FVector2D>& uv = rawMesh.WedgeTexCoords[viewport->GetUVChannel()];

	return (uv);
}

void FUVViewportClient::ApplyTranslation(const FVector& Drag)
{
	TArray<UUVCacheData*> cacheData;
	GetSelectedUV(cacheData);

	FVector2D drag2D = Convert3DtoUV(Drag);
	TArray<FVector2D>& meshUV = GetRawMeshUV();
	
	for (int32 i = 0; i < cacheData.Num(); ++i)
	{
		int32 uvIndex = cacheData[i]->UVIndex;
		FVector2D& uv = meshUV[uvIndex];
		uv += drag2D;
	}

	EndRawMeshChanges();
}

void FUVViewportClient::ApplyRotation(const FRotator& Rotation)
{
	TArray<UUVCacheData*> cacheData;
	GetSelectedUV(cacheData);

	FVector2D barycenter = GetUVSelectionBarycenter();
	FVector barycenter3D = ConvertUVto3D(barycenter);
	TArray<FVector2D>& meshUV = GetRawMeshUV();
	
	for (int32 i = 0; i < cacheData.Num(); ++i)
	{
		int32 uvIndex = cacheData[i]->UVIndex;
		FVector2D& uv = meshUV[uvIndex];
		FVector uv3D = ConvertUVto3D(uv);
		FVector barycenterToUV = uv3D - barycenter3D;
		FVector newPosition3D = barycenter3D + Rotation.RotateVector(barycenterToUV);

		uv = Convert3DtoUV(newPosition3D);
	}

	EndRawMeshChanges();
}

void FUVViewportClient::ApplyScale(const FVector& Scale)
{
	ScaleModifierContext.ApplyScaleDelta(Scale);

	TArray<UUVCacheData*> cacheData;
	GetSelectedUV(cacheData);

	TArray<FVector2D>& meshUV = GetRawMeshUV();

	for (int32 i = 0; i < cacheData.Num(); ++i)
	{
		int32 uvIndex = cacheData[i]->UVIndex;
		FVector2D& uv = meshUV[uvIndex];

		uv = ScaleModifierContext.CalculateUV(uvIndex);
	}

	EndRawMeshChanges();
}

void FUVViewportClient::EndRawMeshChanges()
{
	SUVViewportPtr viewport = GetUVViewport();

	if (!viewport.IsValid()) { return; }

	TWeakPtr<FRPRMeshData> meshData = viewport->GetRPRMeshData();
	if (!meshData.IsValid()) { return; }

	meshData.Pin()->NotifyRawMeshChanges();
}

void FUVViewportClient::DeselectAll()
{
	ModeTools->GetSelectedObjects()->DeselectAll();
}

#undef LOCTEXT_NAMESPACE