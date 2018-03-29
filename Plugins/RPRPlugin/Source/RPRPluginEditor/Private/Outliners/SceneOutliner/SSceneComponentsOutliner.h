#pragma once

#include "DeclarativeSyntaxSupport.h"
#include "SObjectOutliner.h"
#include "Components/StaticMeshComponent.h"
#include "RPRPreviewMeshComponent.h"
#include "RPRMeshDataContainer.h"

class SSceneComponentsOutliner : public SCompoundWidget
{
private:

	typedef SObjectsOutliner<URPRMeshPreviewComponent*> SStaticMeshComponentsOutliner;
	typedef TSharedPtr<SStaticMeshComponentsOutliner> SStaticMeshComponentsOutlinerPtr;

public:

	SLATE_BEGIN_ARGS(SSceneComponentsOutliner) {}
		SLATE_ARGUMENT(FRPRMeshDataContainerWkPtr, MeshDatas)
		SLATE_EVENT(SStaticMeshComponentsOutliner::FOnSelectionChanged, OnSelectionChanged)
	SLATE_END_ARGS()

	void	Construct(const FArguments& InArgs);
	void	Refresh();

	void	SelectAll();
	int32	GetSelectedItems(FRPRMeshDataContainerPtr SelectedMeshComponents) const;

private:

	FText	GetPrettyStaticMeshComponentName(URPRMeshPreviewComponent* StaticMeshComponent) const;
	FText	GetNumberSelectedItemsText() const;
	FReply	OnSelectAllButtonClicked() const;

private:

	SStaticMeshComponentsOutlinerPtr StaticMeshCompsOutliner;
	SStaticMeshComponentsOutliner::FOnSelectionChanged OnSelectionChanged;
	FRPRMeshDataContainerWkPtr MeshDatas;
};

typedef TSharedPtr<SSceneComponentsOutliner> SSceneComponentsOutlinerPtr;