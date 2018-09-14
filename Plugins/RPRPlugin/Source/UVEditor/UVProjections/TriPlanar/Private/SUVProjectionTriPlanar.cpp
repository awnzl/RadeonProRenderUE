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
#include "SUVProjectionTriPlanar.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "IDetailsViewHelper.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "UVProjectionTriPlanarAlgo.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Material/TriPlanarSettings.h"
#include "TriPlanarSettingsEditorLoader.h"

#define LOCTEXT_NAMESPACE "SUVProjectionTriPlanar"

void SUVProjectionTriPlanar::Construct(const FArguments& InArgs)
{
	RPRStaticMeshEditorPtr = InArgs._RPRStaticMeshEditorPtr;

	InitTriPlanarSettings();
	InitUVProjection();
}

void SUVProjectionTriPlanar::OnSectionSelectionChanged()
{
	TryLoadTriPlanarSettings();
}

TSharedRef<SWidget> SUVProjectionTriPlanar::GetAlgorithmSettingsWidget()
{
	return 
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			[
				SNew(SScaleBox)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("SettingsEditor.WarningIcon"))
				]
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.FillWidth(1.0f)
			[
				SNew(SBox)
				.ToolTipText(LOCTEXT("TriPlanarWorkaround", "You can manually duplicate the materials and assign to the different sections, so everything will not be affected by the TriPlanar."))
				[
					SNew(STextBlock)
					.Margin(FMargin(5.0f, 0.0f))
					.AutoWrapText(true)
					.Text(LOCTEXT("TriPlanarWarning", "The TriPlanar modifier only affects the selected RPR materials (not mesh UV)."))
				]
			]
		]
		/*+SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			// Put tri planar widget here
		]*/
	;
}

IUVProjectionAlgorithmPtr SUVProjectionTriPlanar::CreateAlgorithm() const
{
	return (MakeShareable(new FUVProjectionTriPlanarAlgo));
}

void SUVProjectionTriPlanar::OnAlgorithmCompleted(IUVProjectionAlgorithmPtr InAlgorithm, bool bIsSuccess)
{
	if (bIsSuccess)
	{
		FinalizeAlgorithm();
	}
}

UShapePreviewBase* SUVProjectionTriPlanar::GetShapePreview()
{
	return (nullptr);
}

void SUVProjectionTriPlanar::OnPreAlgorithmStart()
{
	UpdateAlgorithmSettings();
}

bool SUVProjectionTriPlanar::RequiredManualApply() const
{
	return (false);
}

void SUVProjectionTriPlanar::InitTriPlanarSettings()
{
	TryLoadTriPlanarSettings();

}

void SUVProjectionTriPlanar::TryLoadTriPlanarSettings()
{
	//TriPlanarDataPerMaterial.Empty();

	FRPRMeshDataContainerPtr selectedMeshDatas = RPRStaticMeshEditorPtr.Pin()->GetSelectedMeshes();
	if (selectedMeshDatas.IsValid())
	{
		selectedMeshDatas->OnEachMeshData([] (FRPRMeshDataPtr MeshData)
		{
			//TriPlanarDataPerMaterial.FindOrAdd()
			//MeshData->GetStaticMesh()->GetMaterial()
		});

		FRPRMeshDataPtr meshData;
		int32 sectionIndex;
		if (selectedMeshDatas->FindFirstSelectedSection(meshData, sectionIndex))
		{
			UMaterialInterface* materialInterface = meshData->GetStaticMesh()->GetMaterial(sectionIndex);

			FTriPlanarSettingsEditorLoader triPlanarSettingsHelper(&Settings);
			triPlanarSettingsHelper.LoadFromMaterial(materialInterface);
		}
	}
}

void SUVProjectionTriPlanar::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FEditPropertyChain* PropertyThatChanged)
{
	StartAlgorithm();
}

void SUVProjectionTriPlanar::UpdateAlgorithmSettings()
{
	FUVProjectionTriPlanarAlgoPtr algo = GetAlgorithm<FUVProjectionTriPlanarAlgo>();
	
	FUVProjectionTriPlanarAlgo::FSettings algoSettings;
	{
		algoSettings.bApply = Settings.bUseTriPlanar;
		algoSettings.Angle = Settings.Angle;
		algoSettings.Scale = Settings.Scale;
	}

	algo->SetSettings(algoSettings);
}

#undef LOCTEXT_NAMESPACE
