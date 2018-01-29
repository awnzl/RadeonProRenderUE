#pragma once

#include "NameTypes.h"
#include "Array.h"
#include "RPRMaterialXmlNode.h"
#include "RPRMaterialXmlUberNode.h"

class FRPRMaterialXmlGraph
{
public:

	bool	ParseFromXml(const class FXmlNode& Node);
	void	Serialize(FRPRMaterialNodeSerializationContext& SerializationContext);

	const FName&							GetName() const;
	FRPRMaterialXmlUberNodePtr				GetUberMaterial() const;
	FRPRMaterialXmlNodePtr					GetFirstMaterial();
	const FRPRMaterialXmlNodePtr			GetFirstMaterial() const;
	const TArray<FRPRMaterialXmlNodePtr>&	GetMaterials() const;

	FRPRMaterialXmlNodePtr				FindNodeByName(const FName& NodeName);

	template<typename NodeType>
	TSharedPtr<NodeType>				FindNodeByName(const FName& NodeName)
	{
		FRPRMaterialXmlNodePtr nodePtr = FindNodeByName(NodeName);
		if (nodePtr.IsValid())
		{
			return (StaticCastSharedPtr<NodeType>(nodePtr));
		}
		return (nullptr);
	}

private:

	void	ParseNodes(const class FXmlNode& Node);

private:
	
	FName Name;
	TArray<FRPRMaterialXmlNodePtr> Nodes;

};