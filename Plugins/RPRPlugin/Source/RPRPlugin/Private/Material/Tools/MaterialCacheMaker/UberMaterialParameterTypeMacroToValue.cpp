#include "UberMaterialParameterTypeMacroToValue.h"
#include "RprSupport.h"


uint32 RPR::FUberMaterialParameterTypeMacroToValue::FindParameterTypeValueByName(const FString& ParameterName)
{
	if (Map().Num() == 0)
	{
#define REGISTER_PARAMETER_TYPE(ParameterTypeName) \
		Map().Add(#ParameterTypeName, ParameterTypeName);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_DIFFUSE_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_DIFFUSE_WEIGHT);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_DIFFUSE_ROUGHNESS);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_WEIGHT);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_ROUGHNESS);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_METALNESS);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_ANISOTROPY);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_ANISOTROPY_ROTATION);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_MODE);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFLECTION_IOR);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFRACTION_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFRACTION_WEIGHT);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFRACTION_ROUGHNESS);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFRACTION_IOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFRACTION_IOR_MODE);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_REFRACTION_THIN_SURFACE);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_COATING_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_COATING_WEIGHT);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_COATING_ROUGHNESS);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_COATING_METALNESS);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_COATING_MODE);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_COATING_IOR);
		
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_EMISSION_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_EMISSION_WEIGHT);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_EMISSION_MODE);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_TRANSPARENCY);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_NORMAL);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_BUMP);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_DISPLACEMENT);

		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_ABSORPTION_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_SCATTER_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_ABSORPTION_DISTANCE);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_SCATTER_DISTANCE);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_SCATTER_DIRECTION);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_WEIGHT);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_SUBSURFACE_COLOR);
		REGISTER_PARAMETER_TYPE(RPRX_UBER_MATERIAL_SSS_MULTISCATTER);
	}

	const uint32* parameterValue = Map().Find(ParameterName);
	return (parameterValue != nullptr ? *parameterValue : 0);
}

TMap<FString, uint32>& RPR::FUberMaterialParameterTypeMacroToValue::Map()
{
	static TMap<FString, uint32> map;
	return (map);
}