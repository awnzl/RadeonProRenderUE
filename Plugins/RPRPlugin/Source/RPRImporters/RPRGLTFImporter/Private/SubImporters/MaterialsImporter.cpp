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

#include "SubImporters/MaterialsImporter.h"
#include "Miscs/RPRMaterialNodeDumper.h"
#include "Material/Tools/UberMaterialPropertyHelper.h"
#include "Factories/Setters/GLTFMaterialParameterSetterFactory.h"
#include "Assets/Factories/RPRMaterialFactory.h"
#include "Factories/Setters/RPRMaterialMapSetter.h"
#include "Factories/Setters/IRPRMaterialParameterSetter.h"
#include "Resources/MaterialResources.h"
#include "RPRCoreSystemResources.h"
#include "RPRSettings.h"
#include "RPR_GLTF_Tools.h"
#include "RPRGLTFImporterModule.h"
#include "RPRCoreModule.h"
#include "AssetRegistryModule.h"
#include "File/RPRFileHelper.h"
#include "Assets/RPRMaterial.h"
#include "ObjectTools.h"
#include "UObject/UObjectGlobals.h"

bool RPR::GLTF::Import::FMaterialsImporter::ImportMaterials(
	const gltf::glTFAssetData& GLTFFileData, 
	RPR::GLTF::FImageResourcesPtr ImageResources, 
	RPR::GLTF::FMaterialResourcesPtr MaterialResources)
{
	TArray<RPRX::FMaterial> materials;
	RPR::FResult status = RPR::GLTF::Import::GetMaterialX(materials);

	if (RPR::GLTF::IsResultFailed(status))
	{
		UE_LOG(LogRPRGLTFImporter, Error, TEXT("Failed to get RPR materials '%s'."), *RPR::GLTF::GetStatusText(status));
		return (false);
	}

	RPRX::FContext rprCtx = IRPRCore::GetResources()->GetRPRXSupportContext();

	for (int32 materialIndex = 0; materialIndex < materials.Num(); ++materialIndex)
	{
		FString materialName = materialIndex < GLTFFileData.materials.size() ? 
			FString(GLTFFileData.materials[materialIndex].name.c_str()) : 
			FString::Printf(TEXT("Material_%d"), materialIndex);

		UE_LOG(LogRPRGLTFImporter, Log, TEXT("Import material '%s'..."), *materialName);

		RPRX::FMaterial nativeRPRMaterial = materials[materialIndex];

		auto& resourceData = MaterialResources->RegisterNewResource(materialIndex);
		resourceData.ResourceRPR = materials[materialIndex];

		URPRMaterial* RPRMaterial = ImportMaterial(materialName, ImageResources, nativeRPRMaterial);
		if (RPRMaterial)
		{
			resourceData.ResourceUE4 = RPRMaterial;
			FAssetRegistryModule::AssetCreated(RPRMaterial);
		}
	}

	return (true);
}

URPRMaterial* RPR::GLTF::Import::FMaterialsImporter::ImportMaterial(
	const FString& MaterialName, 
	RPR::GLTF::FImageResourcesPtr ImageResources, 
	RPRX::FMaterial NativeRPRMaterial)
{
	URPRMaterial* newMaterial = CreateNewMaterial(MaterialName);
	if (newMaterial == nullptr)
	{
		UE_LOG(LogRPRGLTFImporter, Error, TEXT("Cannot create new material '%s'"), *MaterialName);
		return (nullptr);
	}

	FRPRUberMaterialParameters& parameters = newMaterial->MaterialParameters;

	// Create & initialize serialization context
	RPR::GLTF::Importer::FSerializationContext serializationCtx;
	serializationCtx.RPRXContext = IRPRCore::GetResources()->GetRPRXSupportContext();
	serializationCtx.NativeRPRMaterial = NativeRPRMaterial;
	serializationCtx.ImageResources = ImageResources;

	// Use reflection to set each parameter of the Uber material
	UStruct* uberMaterialParameters = FRPRUberMaterialParameters::StaticStruct();
	UProperty* currentParameterProp = uberMaterialParameters->PropertyLink;
	while (currentParameterProp != nullptr)
	{
		FRPRUberMaterialParameterBase* uberMaterialParameter =
			FUberMaterialPropertyHelper::GetParameterBaseFromProperty(&parameters, currentParameterProp);

		if (uberMaterialParameter == nullptr)
		{
			// If the property is not a standard uber material parameter...
			continue;
		}

		UStructProperty* structProperty = Cast<UStructProperty>(currentParameterProp);
		if (structProperty == nullptr)
		{
			// Unsupported property
			continue;
		}

		UStruct* structRef = structProperty->Struct;
		RPR::GLTF::Importer::IRPRMaterialParameterSetterPtr materialParameterSetter =
			RPR::GLTF::Importer::FGLTFMaterialParameterSetterFactory::CreateMaterialParameterSetter(structRef);

		if (materialParameterSetter.IsValid())
		{
			materialParameterSetter->Set(serializationCtx, uberMaterialParameter, uberMaterialParameter->GetRprxParamType());
		}

		currentParameterProp = currentParameterProp->PropertyLinkNext;
	}
	
	URPRMaterialFactory::CopyRPRMaterialParameterToMaterialInstance(newMaterial);
	return (newMaterial);
}

URPRMaterial* RPR::GLTF::Import::FMaterialsImporter::CreateNewMaterial(const FString& MaterialName)
{
	URPRSettings* settings = GetMutableDefault<URPRSettings>();
	FString materialPath = FPaths::Combine(settings->DefaultRootDirectoryForImportedMaterials.Path, MaterialName);
	materialPath = FRPRFileHelper::GenerateUniqueFilename(materialPath);
	FString newMaterialName = FPaths::GetBaseFilename(materialPath);

	UPackage* package = CreatePackage(nullptr, *materialPath);

	URPRMaterialFactory* rprMaterialFactory = NewObject<URPRMaterialFactory>();
	URPRMaterial* rprMaterial = (URPRMaterial*) rprMaterialFactory->FactoryCreateNew(URPRMaterial::StaticClass(), package, *newMaterialName, RF_Public | RF_Standalone | RF_Transactional, nullptr, GWarn);

	package->FullyLoad();
	return rprMaterial;
}
