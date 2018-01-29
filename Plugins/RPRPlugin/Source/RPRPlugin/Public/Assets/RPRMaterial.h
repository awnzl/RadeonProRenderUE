#pragma once

#include "Materials/MaterialInstanceConstant.h"
#include "RPRUberMaterialParameters.h"
#include "RPRMaterial.generated.h"

UCLASS(BlueprintType)
class RPRPLUGIN_API URPRMaterial : public UMaterialInstanceConstant
{
	GENERATED_BODY()

public:

	URPRMaterial();

#if WITH_EDITOR
	virtual void	PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material)
	FRPRUberMaterialParameters	MaterialParameters;
	

#if WITH_EDITORONLY_DATA

	bool	bShouldCacheBeRebuild;

#endif

};