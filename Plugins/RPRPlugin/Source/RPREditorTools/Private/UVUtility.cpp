#include "UVUtility.h"
#include "Engine/StaticMesh.h"
#include "Vector2D.h"
#include "Developer/RawMesh/Public/RawMesh.h"
#include "StaticMeshHelper.h"
#include "RawMesh.h"

const FVector2D FUVUtility::UVsRange(0, 1);

void FUVUtility::ShrinkUVsToBounds(TArray<FVector2D>& UVs, int32 StartOffset)
{
	FVector2D min, max;
	GetUVsBounds(UVs, min, max, StartOffset);
	FVector2D scale = max - min;
	float heighestScale = FMath::Max(scale.X, scale.Y);

	for (int32 i = StartOffset; i < UVs.Num(); ++i)
	{
		UVs[i].X = (UVs[i].X - min.X) / heighestScale;
		UVs[i].Y = (UVs[i].Y - min.Y) / heighestScale;
	}
}

void FUVUtility::GetUVsBounds(const TArray<FVector2D>& UVs, FVector2D& OutMin, FVector2D& OutMax, int32 StartOffset)
{
	checkf(UVs.Num() > 0, TEXT("The UVs array must not be empty!"));

	OutMin = UVs[StartOffset];
	OutMax = UVs[StartOffset];

	for (int32 i = StartOffset + 1; i < UVs.Num(); ++i)
	{
		if (FUVUtility::IsUVValid(UVs[i]))
		{
			if (UVs[i].X < OutMin.X) OutMin.X = UVs[i].X;
			else if (UVs[i].X > OutMax.X) OutMax.X = UVs[i].X;

			if (UVs[i].Y < OutMin.Y) OutMin.Y = UVs[i].Y;
			else if (UVs[i].Y > OutMax.Y) OutMax.Y = UVs[i].Y;
		}
	}
}

void FUVUtility::CenterUVs(TArray<FVector2D>& UVs, int32 StartOffset /*= 0*/)
{
	const FVector2D currentCenter = GetUVsCenter(UVs, StartOffset);
	const FVector2D offset = FVector2D(0.5f, 0.5f) - currentCenter;

	for (int32 i = StartOffset; i < UVs.Num(); ++i)
	{
		UVs[i] = UVs[i] + offset;
	}
}

bool FUVUtility::IsUVTriangleValid(const FVector2D& uvA, const FVector2D& uvB, const FVector2D& uvC)
{
	FVector uvA_3D(uvA.X, uvA.Y, 0);
	FVector uvB_3D(uvB.X, uvB.Y, 0);
	FVector uvC_3D(uvC.X, uvC.Y, 0);

	return (FVector::CrossProduct(uvB_3D - uvA_3D, uvC_3D - uvA_3D).Z <= 0);
}

void FUVUtility::RevertUVTriangle(TArray<FVector2D>& UVs, int32 TriangleIndex)
{
	FVector2D temp = UVs[TriangleIndex + 1];
	UVs[TriangleIndex + 1] = UVs[TriangleIndex + 2];
	UVs[TriangleIndex + 2] = temp;
}

void FUVUtility::RevertAllUVTriangles(TArray<FVector2D>& UVs)
{
	for (int32 i = 0; i < UVs.Num(); i += 3)
	{
		RevertUVTriangle(UVs, i);
	}
}

void FUVUtility::OnEachUVChannel(const FRawMesh& RawMesh, int32 UVChannel, FOnEachUVChannelDelegate Delegate)
{
	if (UVChannel < 0)
	{
		for (int32 uvChannelIdx = 0; uvChannelIdx < MAX_MESH_TEXTURE_COORDS; ++uvChannelIdx)
		{
			if (RawMesh.WedgeTexCoords[uvChannelIdx].Num() > 0)
			{
				Delegate.Execute(uvChannelIdx);
			}
		}
	}
	else
	{
		Delegate.Execute(UVChannel);
	}
}

void FUVUtility::InvertTextureCoordinate(float& TextureCoordinate)
{
	TextureCoordinate = 1.0f - TextureCoordinate;
}

void FUVUtility::InvertUV(FVector2D& InUV)
{
	InvertTextureCoordinate(InUV.X);
	InvertTextureCoordinate(InUV.Y);
}

FVector2D FUVUtility::GetUVsCenter(const TArray<FVector2D>& UVs, int32 StartOffset)
{
	FVector2D min, max;
	GetUVsBounds(UVs, min, max, StartOffset);

	return ((max - min) / 2.0f);
}

FVector FUVUtility::Convert2DTo3D(const FVector2D& UV)
{
	return (FVector(UV.X, 0, UV.Y));
}

FVector2D FUVUtility::Convert3DTo2D(const FVector& UV)
{
	return (FVector2D(UV.X, UV.Z));
}

FVector2D FUVUtility::ApplyTransform(const FTransform& Transform, const FVector2D& UV)
{
	return (Convert3DTo2D(Transform.TransformPosition(Convert2DTo3D(UV))));
}

bool FUVUtility::IsUVValid(const FVector2D& UV)
{
	return FMath::IsFinite(UV.X) && FMath::IsFinite(UV.Y);
}

