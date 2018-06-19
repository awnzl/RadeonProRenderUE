#pragma once
#include "RPRMatParamCopier_MaterialMap.h"

class FRPRMatParamCopier_MaterialCoM : public FRPRMatParamCopier_MaterialMap
{
public:

	virtual void Apply(const FRPRUberMaterialParameters& RPRUberMaterialParameters, UStructProperty* Property, UMaterialEditorInstanceConstant* RPRMaterialEditorInstance) override;

protected:

	virtual bool ShouldUseMap(const FRPRUberMaterialParameters& RPRUberMaterialParameters, UStructProperty* Property, UMaterialEditorInstanceConstant* RPRMaterialEditorInstance) const override;

};