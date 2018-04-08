#pragma once
#include <NE_Common.h>

namespace NuclearEngine
{
	namespace Graphics 
	{
		namespace API 
		{
			/*			
				Base class for all API classes contains useful functionality to decrease code amount
			*/
			template<class GLObjectT, class DXObjectT>
			class CBaseAPI
			{
			protected:
#ifdef 	NE_COMPILE_CORE_OPENGL
				GLObjectT GLObject;
#endif
#ifdef 	NE_COMPILE_DIRECTX11
				DXObjectT DXObject;
#endif
			public:
				inline GLObjectT* GetGLObject()
				{
#ifdef 	NE_COMPILE_CORE_OPENGL
					return &GLObject;
#else
					return nullptr;
#endif
				}
				inline DXObjectT* GetDXObject()
				{
#ifdef 	NE_COMPILE_DIRECTX11
					return &DXObject;
#else
					return nullptr;
#endif
				}
			};
		}
	}
}

//Used for normal no arg methods as binding
#define BASE_API_FUNC_CALL(FuncName) \
if (Graphics::API::Context::isOpenGL3RenderAPI()) {	GLObject.##FuncName(); } \
else if (Graphics::API::Context::isDirectX11RenderAPI()) {	DXObject.##FuncName();}

#define SPECIAL_BASE_API_FUNC_CALL(FuncName, Param1Name) \
if (Graphics::API::Context::isOpenGL3RenderAPI()) {	GLObject.##FuncName(&Param1Name->GLObject); } \
else if (Graphics::API::Context::isDirectX11RenderAPI()) {	DXObject.##FuncName(&Param1Name->DXObject);}

#define STATIC_BASE_API_FUNC_CALL(FuncName) \
if (Graphics::API::Context::isOpenGL3RenderAPI()) { obj->GLObject.##FuncName(&obj->GLObject); } \
else if (Graphics::API::Context::isDirectX11RenderAPI()) {	obj->DXObject.##FuncName(&obj->DXObject);}

#define BASE_API_FUNC_CALL_ARGS(FuncName, ...) \
if (Graphics::API::Context::isOpenGL3RenderAPI()) {	GLObject.##FuncName(__VA_ARGS__); } \
else if (Graphics::API::Context::isDirectX11RenderAPI()) {	DXObject.##FuncName(__VA_ARGS__);}

#define STATIC_BASE_API_FUNC_CALL_ARGS(FuncName, ...) \
if (Graphics::API::Context::isOpenGL3RenderAPI()) { obj->GLObject.##FuncName(&obj->GLObject, __VA_ARGS__); } \
else if (Graphics::API::Context::isDirectX11RenderAPI()) {	obj->DXObject.##FuncName(&obj->DXObject , __VA_ARGS__);}