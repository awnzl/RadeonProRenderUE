#pragma once

#include "UVProjectionAlgorithmBase.h"
#include "Axis.h"

class FUVProjectionCubicAlgo : public FUVProjectionAlgorithmBase
{
public:
	struct FSettings
	{
		FTransform	CubeTransform;
	};

public:

	void	SetSettings(const FSettings& InSettings);
	
	virtual void StartAlgorithm() override;
	virtual void Finalize() override;

	void	StartCubicProjection(int32 MeshIndex, int32 SectionStart, int32 SectionEnd);
	

private:

	void	ProjectUVAlongAxis(int32 MeshIndex, int32 TriangleIndex, int32 VertexIndex, EAxis::Type AxisComponentA, EAxis::Type AxisComponentB);
	
private:

	FSettings	Settings;

};