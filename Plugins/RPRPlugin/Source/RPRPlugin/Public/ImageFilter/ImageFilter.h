#pragma once

#include "CoreMinimal.h"

#include "RadeonProRender.h"
#define RIF_STATIC_LIBRARY 0
#include "RadeonImageFilters.h"

enum class RifFilterType
{
	BilateralDenoise,
	LwrDenoise,
	EawDenoise,
	MlDenoise,
	MlDenoiseColorOnly
};

enum RifFilterInput
{
	RifColor,
	RifNormal,
	RifDepth,
	RifWorldCoordinate,
	RifObjectId,
	RifTrans,
	RifAlbedo,
	RifMaxInput
};

enum class RifParamType
{
	RifInt,
	RifFloat
};

union RifData
{
	rif_int   i;
	rif_float f;
};

struct RifParam
{
	RifParamType mType;
	RifData      mData;
};

class RifContextWrapper;
class RifFilterWrapper;

class ImageFilter final
{
	TUniquePtr<RifContextWrapper> mRifContext;
	TUniquePtr<RifFilterWrapper>  mRifFilter;

	uint32  mWidth;
	uint32  mHeight;
	FString mModelsPath;

public:
	explicit ImageFilter(const rpr_context rprContext, uint32 width, uint32 height,
		const FString& modelsPath = FString());
	~ImageFilter();

	void CreateFilter(RifFilterType rifFilteType, bool useOpenImageDenoise = false);
	void DeleteFilter();

	void AddInput(RifFilterInput inputId, const rpr_framebuffer rprFrameBuffer, float sigma) const;
	void AddInput(RifFilterInput inputId, float* memPtr, size_t size, float sigma) const;
	void AddParam(FString name, RifParam param) const;

	void AttachFilter() const;

	void Run() const;

	TArray<float> GetData() const;
};

enum class RifContextType
{
	RifContextCPU,
	RifContextGPU,
	RifContextGPUMetal
};

class RifContextWrapper
{
protected:
	rif_context mRifContextHandle = nullptr;
	rif_command_queue mRifCommandQueueHandle = nullptr;
	rif_image mOutputRifImage = nullptr;
	RifContextType mContextType = RifContextType::RifContextCPU;

public:
	virtual ~RifContextWrapper();

	const rif_context Context() const;
	const rif_command_queue Queue() const;
	const rif_image Output() const;
	const RifContextType ContextType() const;

	void CreateOutput(const rif_image_desc& desc);

	virtual rif_image CreateRifImage(const rpr_framebuffer rprFrameBuffer, const rif_image_desc& desc) const = 0;
	void UpdateInputs(const RifFilterWrapper* rifFilter) const;

protected:
	virtual TArray<rpr_char> GetRprCachePath(rpr_context rprContext) const final;
};

class RifContextGPU final : public RifContextWrapper
{
	const rif_backend_api_type rifBackendApiType = RIF_BACKEND_API_OPENCL;

public:
	explicit RifContextGPU(const rpr_context rprContext);
	virtual ~RifContextGPU();

	virtual rif_image CreateRifImage(const rpr_framebuffer rprFrameBuffer, const rif_image_desc& desc) const override;
};

class RifContextGPUMetal final : public RifContextWrapper
{
	const rif_backend_api_type rifBackendApiType = RIF_BACKEND_API_METAL;

public:
	explicit RifContextGPUMetal(const rpr_context rprContext);
	virtual ~RifContextGPUMetal();

	virtual rif_image CreateRifImage(const rpr_framebuffer rprFrameBuffer, const rif_image_desc& desc) const override;
};

class RifContextCPU final : public RifContextWrapper
{
	const rif_backend_api_type rifBackendApiType = RIF_BACKEND_API_OPENCL;

public:
	explicit RifContextCPU(const rpr_context rprContext);
	virtual ~RifContextCPU();

	virtual rif_image CreateRifImage(const rpr_framebuffer rprFrameBuffer, const rif_image_desc& desc) const override;
};



struct RifInput
{
	rif_image mRifImage = nullptr;
	float     mSigma = 0.0f;

	RifInput(rif_image rifImage, float sigma);
	virtual ~RifInput();
	virtual void Update() = 0;
};

using RifInputPtr = TSharedPtr<RifInput>;

struct RifInputGPU : public RifInput
{
	RifInputGPU(rif_image rifImage, float sigma);
	virtual ~RifInputGPU();
	void Update() override;
};

struct RifInputGPUCPU : public RifInput
{
	rpr_framebuffer mRprFrameBuffer = nullptr;

	RifInputGPUCPU(rif_image rifImage, const rpr_framebuffer rprFrameBuffer, float sigma);
	virtual ~RifInputGPUCPU();
	void Update() override;
};

struct RifInputCPU : public RifInput
{
	float* mMemPtr = nullptr;
	size_t mSize = 0;

	RifInputCPU(rif_image rifImage, float* memPtr, size_t size, float sigma);
	virtual ~RifInputCPU();
	void Update() override;
};

class RifFilterWrapper
{
	friend class RifContextWrapper;
	friend class RifContextCPU;

protected:
	rif_image_filter mRifImageFilterHandle = nullptr;

	TArray<rif_image_filter> mAuxFilters;
	TArray<rif_image> mAuxImages;

	TMap<RifFilterInput, RifInputPtr> mInputs;
	TMap<FString, RifParam> mParams;

public:
	virtual ~RifFilterWrapper();

	void AddInput(RifFilterInput inputId, const rif_image rifImage, float sigma);
	void AddInput(RifFilterInput inputId, const rif_image rifImage, const rpr_framebuffer rprFrameBuffer, float sigma);
	void AddInput(RifFilterInput inputId, const rif_image rifImage, float* memPtr, size_t size, float sigma);
	void AddParam(FString name, RifParam param);

	virtual void AttachFilter(const RifContextWrapper* rifContext) = 0;
	virtual void DetachFilter(const RifContextWrapper* rifContext) noexcept final;

	void ApplyParameters() const;

protected:
	void SetupVarianceImageFilter(const rif_image_filter inputFilter, const rif_image outVarianceImage) const;
};

class RifFilterBilateral final : public RifFilterWrapper
{
	// vector representation of inputs is needed to feed library
	TArray<rif_image> inputImages;
	TArray<float> sigmas;

public:
	explicit RifFilterBilateral(const RifContextWrapper* rifContext);
	virtual ~RifFilterBilateral();

	virtual void AttachFilter(const RifContextWrapper* rifContext) override;
};

class RifFilterLwr final : public RifFilterWrapper
{
	enum
	{
		ColorVar,
		NormalVar,
		DepthVar,
		TransVar,
		AuxFilterMax
	};

	enum
	{
		ColorVarianceImage,
		NormalVarianceImage,
		DepthVarianceImage,
		TransVarianceImage,
		AuxImageMax
	};

public:
	explicit RifFilterLwr(const RifContextWrapper* rifContext, uint32 width, uint32 height);
	virtual ~RifFilterLwr();

	virtual void AttachFilter(const RifContextWrapper* rifContext) override;
};

class RifFilterEaw final : public RifFilterWrapper
{
	enum
	{
		ColorVar,
		Mlaa,
		AuxFilterMax
	};

	enum
	{
		ColorVarianceImage,
		DenoisedOutputImage,
		AuxImageMax
	};

public:
	explicit RifFilterEaw(const RifContextWrapper* rifContext, uint32 width, uint32 height);
	virtual ~RifFilterEaw();

	virtual void AttachFilter(const RifContextWrapper* rifContext) override;
};

class RifFilterMl final : public RifFilterWrapper
{
	enum
	{
		NormalsRemapFilter,
		DepthRemapFilter,
		OutputResampleFilter,
		AuxFilterMax
	};

	enum
	{
		MlOutputRifImage,
		AuxImageMax
	};

public:
	explicit RifFilterMl(const RifContextWrapper* rifContext, uint32 width, uint32 height,
		const FString& modelsPath, bool useOpenImageDenoise);
	virtual ~RifFilterMl();

	virtual void AttachFilter(const RifContextWrapper* rifContext) override;
};

class RifFilterMlColorOnly final : public RifFilterWrapper
{
	enum
	{
		OutputResampleFilter,
		AuxFilterMax
	};

	enum
	{
		MlOutputRifImage,
		AuxImageMax
	};

public:
	explicit RifFilterMlColorOnly(const RifContextWrapper* rifContext, uint32 width, uint32 height,
		const FString& modelsPath, bool useOpenImageDenoise);
	virtual ~RifFilterMlColorOnly();

	virtual void AttachFilter(const RifContextWrapper* rifContext) override;
};