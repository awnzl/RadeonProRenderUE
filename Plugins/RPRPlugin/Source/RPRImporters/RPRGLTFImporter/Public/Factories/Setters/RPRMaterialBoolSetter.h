#pragma once
#include "Factories/Setters/RPRMaterialParameterSetter.h"

namespace RPR
{
	namespace GLTF
	{
		namespace Importer
		{
			class FRPRMaterialBoolSetter : public FRPRMaterialParameterSetter
			{

			protected:

				virtual bool SetParameterValue(FSerializationContext& SerializationCtx, FRPRUberMaterialParameterBase* UberParameter, RPRX::EMaterialParameterType ParameterType, RPRX::FParameter Parameter) override;
				virtual bool IsParameterTypeSupported(RPRX::EMaterialParameterType MaterialParameterType) override;

			};
		}
	}
}
