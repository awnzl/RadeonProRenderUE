/*************************************************************************
* Copyright 2020 Advanced Micro Devices
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/

#include "RPRCpStaticMesh.h"
#include "RPRCpStaticMeshMacros.h"

FPositionVertexBuffer& FRPRCpStaticMesh::GetPositionVertexBuffer(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (const_cast<FPositionVertexBuffer&>(GetPositionVertexBufferConst(StaticMeshLODResource)));
}

FStaticMeshVertexBuffer& FRPRCpStaticMesh::GetStaticMeshVertexBuffer(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (const_cast<FStaticMeshVertexBuffer&>(GetStaticMeshVertexBufferConst(StaticMeshLODResource)));
}

FColorVertexBuffer& FRPRCpStaticMesh::GetColorVertexBuffer(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (const_cast<FColorVertexBuffer&>(GetColorVertexBufferConst(StaticMeshLODResource)));
}


#if ENGINE_MINOR_VERSION == 18


const FPositionVertexBuffer& FRPRCpStaticMesh::GetPositionVertexBufferConst(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (StaticMeshLODResource.PositionVertexBuffer);
}

const FStaticMeshVertexBuffer& FRPRCpStaticMesh::GetStaticMeshVertexBufferConst(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (StaticMeshLODResource.VertexBuffer);
}

const FColorVertexBuffer& FRPRCpStaticMesh::GetColorVertexBufferConst(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (StaticMeshLODResource.ColorVertexBuffer);
}

FVertexBufferRHIRef& FRPRCpStaticMesh::GetTexCoordVertexBufferRHI(FStaticMeshVertexBuffer& VertexBuffer)
{
	return (VertexBuffer.VertexBufferRHI);
}

int32 FRPRCpStaticMesh::GetTexCoordBufferSize(FStaticMeshVertexBuffer& VertexBuffer)
{
	SELECT_UV_TYPE(
		VertexBuffer.GetUseHighPrecisionTangentBasis(),
		VertexBuffer.GetUseHighPrecisionTangentBasis(),
		VertexBuffer.GetNumTexCoords(),
		{
			return (sizeof(UVType) * VertexBuffer.GetNumTexCoords() * VertexBuffer.GetNumVertices());
		}
	);
	return (INDEX_NONE);
}

uint8* FRPRCpStaticMesh::AllocateAndCopyTexCoordDatas(FStaticMeshVertexBuffer& VertexBuffer)
{
	SELECT_UV_TYPE(
		VertexBuffer.GetUseHighPrecisionTangentBasis(),
		VertexBuffer.GetUseFullPrecisionUVs(),
		VertexBuffer.GetNumTexCoords(),
		{
			int32 sizeofVertexBuffer = sizeof(UVType);
			int32 sizeToCopy = VertexBuffer.GetNumVertices() * sizeofVertexBuffer;
			uint8* initialData = new uint8[sizeToCopy];
			FMemory::Memcpy(initialData, VertexBuffer.GetRawVertexData(), sizeToCopy);
			return (initialData);
		}
	);
	return (nullptr);
}

void FRPRCpStaticMesh::TransformUV_RenderThread(const FTransform2D& NewTransform, int32 UVChannel, FStaticMeshVertexBuffer& VertexBuffer, uint8* InitialDatas)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_TransformUV);

	SELECT_UV_TYPE(
		VertexBuffer.GetUseHighPrecisionTangentBasis(),
		VertexBuffer.GetUseFullPrecisionUVs(),
		VertexBuffer.GetNumTexCoords(),
		{
			const int32 sizeofVertexBuffer = sizeof(UVType);
			const uint32 numVertices = VertexBuffer.GetNumVertices();
			const int32 stride = VertexBuffer.GetStride();

			VertexType* staticMeshVertexBuffer = (UVType*)RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, numVertices * sizeofVertexBuffer, RLM_WriteOnly);
			{
				for (uint32 vertexIndex = 0; vertexIndex < numVertices ; ++vertexIndex)
				{
					UVType* uvBuffer = reinterpret_cast<UVType*>(InitialDatas + vertexIndex * stride);
					FVector2D initialUV = uvBuffer->GetUV(UVChannel);
					FVector2D newUV = NewTransform.TransformPoint(initialUV);
					staticMeshVertexBuffer[vertexIndex].SetUV(UVChannel, newUV);
				}
			}
		}
	);

	RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
}


#elif ENGINE_MINOR_VERSION >= 19


const FPositionVertexBuffer& FRPRCpStaticMesh::GetPositionVertexBufferConst(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (StaticMeshLODResource.VertexBuffers.PositionVertexBuffer);
}

const FStaticMeshVertexBuffer& FRPRCpStaticMesh::GetStaticMeshVertexBufferConst(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (StaticMeshLODResource.VertexBuffers.StaticMeshVertexBuffer);
}

const FColorVertexBuffer& FRPRCpStaticMesh::GetColorVertexBufferConst(const FStaticMeshLODResources& StaticMeshLODResource)
{
	return (StaticMeshLODResource.VertexBuffers.ColorVertexBuffer);
}

FVertexBufferRHIRef& FRPRCpStaticMesh::GetTexCoordVertexBufferRHI(FStaticMeshVertexBuffer& VertexBuffer)
{
	return (VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI);
}

int32 FRPRCpStaticMesh::GetTexCoordBufferSize(FStaticMeshVertexBuffer& VertexBuffer)
{
	return (VertexBuffer.GetTexCoordSize());
}

uint8* FRPRCpStaticMesh::AllocateAndCopyTexCoordDatas(FStaticMeshVertexBuffer& VertexBuffer)
{
	const int32 bufferSize = FRPRCpStaticMesh::GetTexCoordBufferSize(VertexBuffer);
	uint8* initialData = new uint8[bufferSize];
	FMemory::Memcpy(initialData, VertexBuffer.GetTexCoordData(), bufferSize);
	return (initialData);
}

void FRPRCpStaticMesh::TransformUV_RenderThread(const FTransform2D& NewTransform, int32 UVChannel, FStaticMeshVertexBuffer& VertexBuffer, uint8* InitialDatas)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_TransformUV);

	SELECT_UV_TYPE(
		VertexBuffer.GetUseHighPrecisionTangentBasis(),
		VertexBuffer.GetUseFullPrecisionUVs(),
		VertexBuffer.GetNumTexCoords(),
		{
			const uint32 numVertices = VertexBuffer.GetNumVertices();
			const int32 stride = sizeof(UVType) * VertexBuffer.GetNumTexCoords();

			uint8* staticMeshVertexBuffer = (uint8*)RHILockVertexBuffer(FRPRCpStaticMesh::GetTexCoordVertexBufferRHI(VertexBuffer), 0, VertexBuffer.GetTexCoordSize(), RLM_WriteOnly);
			{
				for (uint32 vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
				{
					UVType* initialUVBuffer = reinterpret_cast<UVType*>(InitialDatas + (vertexIndex * stride));
					FVector2D initialUV = initialUVBuffer[UVChannel].GetUV();

					FVector2D newUV = NewTransform.TransformPoint(initialUV);

					UVType* dstUVBuffer = reinterpret_cast<UVType*>(staticMeshVertexBuffer + (vertexIndex * stride));
					dstUVBuffer[UVChannel].SetUV(newUV);
				}
			}
		}
	);

	RHIUnlockVertexBuffer(FRPRCpStaticMesh::GetTexCoordVertexBufferRHI(VertexBuffer));
}

#endif
