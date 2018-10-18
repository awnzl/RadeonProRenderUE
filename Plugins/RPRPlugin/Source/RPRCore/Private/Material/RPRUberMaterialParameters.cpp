/**********************************************************************
* Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
********************************************************************/
#include "Material/RPRUberMaterialParameters.h"
#include "Material/Tools/MaterialCacheMaker/ParameterSetters/Specials/NormalMapParameterSetter.h"

#define LOCTEXT_NAMESPACE "RPRUberMaterialParameters"

static bool CanUseOnlyIfValidModeSet(const FRPRUberMaterialParameterBase* Parameter, FRPRMaterialEnum* EnumParameter, uint8 ExpectedValidMode)
{
	return (EnumParameter->EnumValue == ExpectedValidMode);
}

static void ApplyNormalMap(RPRX::MaterialParameter::FArgs& Args)
{
    RPRX::FNormalMapParameterSetter normalMapParameterSetter;
    normalMapParameterSetter.ApplyParameterX(Args);    
}

static bool CantUseParameter(const FRPRUberMaterialParameterBase* MaterialParameter)
{
	return (false);
}

FRPRUberMaterialParameters::FRPRUberMaterialParameters()
	// Diffuse
	: Diffuse_Weight(		TEXT("diffuse.weight"),		RPRX_UBER_MATERIAL_DIFFUSE_WEIGHT,		ESupportMode::FullySupported, 1.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Diffuse_Color(		TEXT("diffuse.color"),		RPRX_UBER_MATERIAL_DIFFUSE_COLOR,		ESupportMode::FullySupported, 1.0f)
	, Diffuse_Normal(		TEXT("diffuse.normal"),		RPRX_UBER_MATERIAL_DIFFUSE_NORMAL,		ESupportMode::FullySupported, FCanUseParameter(), FApplyParameter::CreateStatic(ApplyNormalMap))
	, Diffuse_Roughness(	TEXT("diffuse.roughness"),	RPRX_UBER_MATERIAL_DIFFUSE_ROUGHNESS,	ESupportMode::NotSupported,	0.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)

	// Reflection
	, Reflection_Weight(				TEXT("reflection.weight"),				RPRX_UBER_MATERIAL_REFLECTION_WEIGHT,				ESupportMode::FullySupported,		1.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Reflection_Color(					TEXT("reflection.color"),				RPRX_UBER_MATERIAL_REFLECTION_COLOR,				ESupportMode::PreviewNotSupported, 1.0f)
	, Reflection_Normal(				TEXT("reflection.normal"),				RPRX_UBER_MATERIAL_REFLECTION_NORMAL,				ESupportMode::PreviewNotSupported,  FCanUseParameter(), FApplyParameter::CreateStatic(ApplyNormalMap))
	, Reflection_Roughness(				TEXT("reflection.roughness"),			RPRX_UBER_MATERIAL_REFLECTION_ROUGHNESS,			ESupportMode::FullySupported,		0.25f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Reflection_Anisotropy(			TEXT("reflection.anisotropy"),			RPRX_UBER_MATERIAL_REFLECTION_ANISOTROPY,			ESupportMode::PreviewNotSupported,	0.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Reflection_AnisotropyRotation(	TEXT("reflection.anisotropyRotation"),	RPRX_UBER_MATERIAL_REFLECTION_ANISOTROPY_ROTATION,	ESupportMode::PreviewNotSupported,	0.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)

	, Reflection_Metalness(	TEXT("reflection.metalness"),	RPRX_UBER_MATERIAL_REFLECTION_METALNESS,	ESupportMode::FullySupported,		1.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4,	FCanUseParameter::CreateStatic(CanUseOnlyIfValidModeSet, &Reflection_Mode, (uint8) ERPRReflectionMode::Metalness))
	, Reflection_Ior(		TEXT("reflection.ior"),			RPRX_UBER_MATERIAL_REFLECTION_IOR,			ESupportMode::PreviewNotSupported,	1.5f,	ERPRMCoMapC1InterpretationMode::AsFloat4,	FCanUseParameter::CreateStatic(CanUseOnlyIfValidModeSet, &Reflection_Mode, (uint8) ERPRReflectionMode::PBR))
	
	, Reflection_Mode(FRPRMaterialEnum::Create<ERPRReflectionMode>(TEXT("reflection.mode"), RPRX_UBER_MATERIAL_REFLECTION_MODE, ESupportMode::PreviewNotSupported, ERPRReflectionMode::PBR))

	// Refraction
	, Refraction_Color(						TEXT("refraction.color"),				RPRX_UBER_MATERIAL_REFRACTION_COLOR,				ESupportMode::PreviewNotSupported, 1.0f)
	, Refraction_Weight(					TEXT("refraction.weight"),				RPRX_UBER_MATERIAL_REFRACTION_WEIGHT,				ESupportMode::PreviewNotSupported, 0.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Refraction_Roughness(					TEXT("refraction.roughness"),			RPRX_UBER_MATERIAL_REFRACTION_ROUGHNESS,			ESupportMode::PreviewNotSupported, 0.5f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Refraction_Absorption_Color(			TEXT("refraction.absorptionColor"),		RPRX_UBER_MATERIAL_REFRACTION_ABSORPTION_COLOR,		ESupportMode::PreviewNotSupported, 0.0f)
	, Refraction_Absorption_Distance(		TEXT("refraction.absorptionDistance"),	RPRX_UBER_MATERIAL_REFRACTION_ABSORPTION_DISTANCE,	ESupportMode::PreviewNotSupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Refraction_Caustics(					TEXT("refraction.caustics"),			RPRX_UBER_MATERIAL_REFRACTION_CAUSTICS,				ESupportMode::PreviewNotSupported, false)
	, Refraction_Ior(						TEXT("refraction.ior"),					RPRX_UBER_MATERIAL_REFRACTION_IOR,					ESupportMode::PreviewNotSupported, 1.5f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Refraction_IsThinSurface(				TEXT("refraction.thinSurface"),			RPRX_UBER_MATERIAL_REFRACTION_THIN_SURFACE,			ESupportMode::PreviewNotSupported, false)

	// Coating
	, Coating_Color(						TEXT("coating.color"),				RPRX_UBER_MATERIAL_COATING_COLOR,					ESupportMode::PreviewNotSupported, 1.0f)
	, Coating_Transmission_Color(			TEXT("coating.transmissioncolor"),	RPRX_UBER_MATERIAL_COATING_TRANSMISSION_COLOR,		ESupportMode::PreviewNotSupported, 1.0f)
	, Coating_Normal(						TEXT("coating.normal"),				RPRX_UBER_MATERIAL_COATING_NORMAL,					ESupportMode::PreviewNotSupported, FCanUseParameter(), FApplyParameter::CreateStatic(ApplyNormalMap))
	, Coating_Thickness(					TEXT("coating.thickness"),			RPRX_UBER_MATERIAL_COATING_THICKNESS,				ESupportMode::PreviewNotSupported, 1.0f)
	, Coating_Weight(						TEXT("coating.weight"),				RPRX_UBER_MATERIAL_COATING_WEIGHT,					ESupportMode::PreviewNotSupported, 0.0f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Coating_Roughness(					TEXT("coating.roughness"),			RPRX_UBER_MATERIAL_COATING_ROUGHNESS,				ESupportMode::PreviewNotSupported, 0.5f,	ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Coating_Ior(							TEXT("coating.ior"),				RPRX_UBER_MATERIAL_COATING_IOR,						ESupportMode::PreviewNotSupported, 1.5f,	ERPRMCoMapC1InterpretationMode::AsFloat4)

	, Coating_Mode(FRPRMaterialEnum::Create<ERPRCoatingMode>(TEXT("coating.mode"), RPRX_UBER_MATERIAL_COATING_MODE, ESupportMode::PreviewNotSupported, ERPRCoatingMode::PBR))

	// Emission
	, Emission_Color(	TEXT("emission.color"),		RPRX_UBER_MATERIAL_EMISSION_COLOR,	ESupportMode::FullySupported, 1.0f)
	, Emission_Weight(	TEXT("emission.weight"),	RPRX_UBER_MATERIAL_EMISSION_WEIGHT, ESupportMode::FullySupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)

	, Emission_Mode(FRPRMaterialEnum::Create<ERPREmissionMode>(TEXT("emission.mode"), RPRX_UBER_MATERIAL_EMISSION_MODE, ESupportMode::FullySupported, ERPREmissionMode::SingleSided))

	// Miscs
	, Transparency(	TEXT("transparency"),	RPRX_UBER_MATERIAL_TRANSPARENCY, ESupportMode::PreviewNotSupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Displacement(	TEXT("displacement"),	RPRX_UBER_MATERIAL_DISPLACEMENT, ESupportMode::PreviewNotSupported)

	// SSS
	, SSS_Weight(				TEXT("sss.weight"),					RPRX_UBER_MATERIAL_SSS_WEIGHT,					ESupportMode::PreviewNotSupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)
	, SSS_Scatter_Color(		TEXT("sss.scatterColor"),			RPRX_UBER_MATERIAL_SSS_SCATTER_COLOR,			ESupportMode::PreviewNotSupported, 0.0f)
	, SSS_Scatter_Distance(		TEXT("sss.scatterDistance"),		RPRX_UBER_MATERIAL_SSS_SCATTER_DISTANCE,		ESupportMode::PreviewNotSupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)
	, SSS_Scatter_Direction(	TEXT("sss.scatterDirection"),		RPRX_UBER_MATERIAL_SSS_SCATTER_DIRECTION,		ESupportMode::PreviewNotSupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)
	, SSS_IsMultiScatter(		TEXT("sss.multiScatter"),			RPRX_UBER_MATERIAL_SSS_MULTISCATTER,			ESupportMode::PreviewNotSupported, true)

	// Backscatter
	, Backscatter_Weight(	TEXT("backscatter.weight"),	RPRX_UBER_MATERIAL_BACKSCATTER_WEIGHT,	ESupportMode::PreviewNotSupported, 0.0f, ERPRMCoMapC1InterpretationMode::AsFloat4)
	, Backscatter_Color(	TEXT("backscatter.color"),	RPRX_UBER_MATERIAL_BACKSCATTER_COLOR,	ESupportMode::PreviewNotSupported, 1.0f)

	// Sheen (Not implemented yet)
	//, Sheen(		TEXT("sheen"),			RPRX_UBER_MATERIAL_SHEEN, ESupportMode::PreviewNotSupported, //)
	//, Sheen_Tint(	TEXT("sheen.tint"),		RPRX_UBER_MATERIAL_SHEEN_TINT, ESupportMode::PreviewNotSupported, //)
	//, Sheen_Weight(	TEXT("sheen.weight"),	RPRX_UBER_MATERIAL_SHEEN_WEIGHT, ESupportMode::PreviewNotSupported, //)
{
#if WITH_EDITOR
	SetupEditorSettings();
#endif
}


#if WITH_EDITOR

void FRPRUberMaterialParameters::SetupEditorSettings()
{
	Diffuse_Weight.GetConstantRestriction().SetRange01();
	Reflection_Weight.GetConstantRestriction().SetRange01();
	Reflection_Metalness.GetConstantRestriction().SetRange01();
	Coating_Weight.GetConstantRestriction().SetRange01();
	Emission_Weight.GetConstantRestriction().SetRange01();
	Transparency.GetConstantRestriction().SetRange01();
	SSS_Weight.GetConstantRestriction().SetRange01();
	Backscatter_Weight.GetConstantRestriction().SetRange01();
	Refraction_Weight.GetConstantRestriction().SetRange01();

	Diffuse_Roughness.GetConstantRestriction().SetRange01();
	Reflection_Roughness.GetConstantRestriction().SetRange01();
	Refraction_Roughness.GetConstantRestriction().SetRange01();
	Coating_Roughness.GetConstantRestriction().SetRange01();

	Coating_Thickness.GetConstantRestriction().SetMinimum(0.0f);
	Coating_Thickness.GetConstantRestriction().SetMinimum(10.0f);

	Reflection_Anisotropy.GetConstantRestriction().SetMinimum(-1.0f);
	Reflection_Anisotropy.GetConstantRestriction().SetMaximum(1.0f);

	Reflection_AnisotropyRotation.GetConstantRestriction().SetMinimum(-1.0f);
	Reflection_AnisotropyRotation.GetConstantRestriction().SetMaximum(1.0f);

	Reflection_Ior.GetConstantRestriction().SetMinimum(0.0f);
	Refraction_Ior.GetConstantRestriction().SetMinimum(0.0f);
	Coating_Ior.GetConstantRestriction().SetMinimum(0.0f);

	SSS_Scatter_Distance.GetConstantRestriction().SetMinimum(0.0f);
	SSS_Scatter_Direction.GetConstantRestriction().SetRange(-1.0f, 1.0f);

}

#endif // WITH_EDITOR


#undef LOCTEXT_NAMESPACE
