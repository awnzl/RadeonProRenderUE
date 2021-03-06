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

#include "Helpers/RPRShapeHelpers.h"
#include "RadeonProRender.h"
#include "Helpers/RPRHelpers.h"

DEFINE_LOG_CATEGORY(LogRPRShapeHelpers)

namespace RPR
{
	namespace Shape
	{

		template<typename T>
		RPR::FResult GetInfoNoAlloc(RPR::FShape Shape, RPR::EShapeInfo Info, T* OutValue)
		{
			return RPR::Generic::GetInfoNoAlloc(rprShapeGetInfo, Shape, Info, OutValue);
		}

		template<typename T>
		RPR::FResult GetInfoToArray(RPR::FShape Shape, RPR::EShapeInfo Info, TArray<T>& OutValue)
		{
			return (RPR::Generic::GetInfoToArray(rprShapeGetInfo, Shape, Info, OutValue));
		}

		//////////////////////////////////////////////////////////////////////////

		RPR::FResult GetName(RPR::FShape Shape, FString& OutName)
		{
			return RPR::Generic::GetObjectName(rprShapeGetInfo, Shape, OutName);
		}

		FString GetName(RPR::FShape Shape)
		{
			FString name;
			RPR::FResult status = GetName(Shape, name);
			if (RPR::IsResultFailed(status))
			{
				name = TEXT("[Unknown]");
			}
			if (name.IsEmpty())
			{
				name = TEXT("[Undefined]");
			}
			return (name);
		}

		RPR::FResult GetMaterial(RPR::FShape Shape, RPR::FMaterialNode& OutMaterialNode)
		{
			return (GetInfoNoAlloc(Shape, EShapeInfo::Material, &OutMaterialNode));
		}

		RPR::FResult GetType(RPR::FShape Shape, RPR::EShapeType& OutShapeType)
		{
			return GetInfoNoAlloc(Shape, EShapeInfo::Type, &OutShapeType);
		}

		RPR::FResult GetWorldTransform(RPR::FShape Shape, FTransform& OutTransform)
		{
			return RPR::Generic::GetObjectTransform(rprShapeGetInfo, Shape, EShapeInfo::Transform, OutTransform);
		}

		RPR::FResult SetTransform(RPR::FShape Shape, const FTransform& Transform)
		{
			RadeonProRender::matrix matrix = BuildMatrixWithScale(Transform);

			RPR::FResult status = rprShapeSetTransform(Shape, RPR_TRUE, &matrix.m00);

			UE_LOG(LogRPRTools_Step, Verbose,
				TEXT("rprShapeSetTransform(shape=%s, tranpose=true, matrix=%s) -> %d"),
				*RPR::Shape::GetName(Shape),
				*Transform.ToString(),
				status
			);

			return status;
		}

		RPR::FResult GetInstanceBaseShape(RPR::FShape Shape, RPR::FShape& OutShape)
		{
			return rprInstanceGetBaseShape(Shape, &OutShape);
		}

		RPR::FResult SetMaterial(FShape Shape, RPR::FMaterialNode MaterialNode)
		{
			UE_LOG(LogRPRTools_Step, Verbose, 
				TEXT("rprShapeSetMaterial(%s, %s)"), 
				*RPR::Shape::GetName(Shape), 
				*RPR::RPRMaterial::GetNodeName(MaterialNode)
			);

			return rprShapeSetMaterial(Shape, MaterialNode);
		}

	} // namespace Shape
} // namespace RPR

