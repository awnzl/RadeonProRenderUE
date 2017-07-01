// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef RPRPLUGIN_actorRPRHandler_2_generated_h
#error "actorRPRHandler_2.generated.h already included, missing '#pragma once' in actorRPRHandler_2.h"
#endif
#define RPRPLUGIN_actorRPRHandler_2_generated_h

#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execRenderTestScene) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RenderTestScene(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execInit) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->Init(); \
		P_NATIVE_END; \
	}


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execRenderTestScene) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RenderTestScene(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execInit) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->Init(); \
		P_NATIVE_END; \
	}


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAactorRPRHandler_2(); \
	friend RPRPLUGIN_API class UClass* Z_Construct_UClass_AactorRPRHandler_2(); \
public: \
	DECLARE_CLASS(AactorRPRHandler_2, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/RPRPlugin"), NO_API) \
	DECLARE_SERIALIZER(AactorRPRHandler_2) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_INCLASS \
private: \
	static void StaticRegisterNativesAactorRPRHandler_2(); \
	friend RPRPLUGIN_API class UClass* Z_Construct_UClass_AactorRPRHandler_2(); \
public: \
	DECLARE_CLASS(AactorRPRHandler_2, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/RPRPlugin"), NO_API) \
	DECLARE_SERIALIZER(AactorRPRHandler_2) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AactorRPRHandler_2(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AactorRPRHandler_2) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AactorRPRHandler_2); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AactorRPRHandler_2); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AactorRPRHandler_2(AactorRPRHandler_2&&); \
	NO_API AactorRPRHandler_2(const AactorRPRHandler_2&); \
public:


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AactorRPRHandler_2(AactorRPRHandler_2&&); \
	NO_API AactorRPRHandler_2(const AactorRPRHandler_2&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AactorRPRHandler_2); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AactorRPRHandler_2); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AactorRPRHandler_2)


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_PRIVATE_PROPERTY_OFFSET
#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_13_PROLOG
#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_PRIVATE_PROPERTY_OFFSET \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_RPC_WRAPPERS \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_INCLASS \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_PRIVATE_PROPERTY_OFFSET \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_INCLASS_NO_PURE_DECLS \
	RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID RPR_Plugins_RPRPlugin_Source_RPRPlugin_Public_actorRPRHandler_2_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
