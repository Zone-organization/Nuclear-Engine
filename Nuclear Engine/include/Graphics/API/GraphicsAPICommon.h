#pragma once
#include <NE_Common.h>

#define BASE_API(BaseName) \
OpenGL::GL##BaseName GLObject;\
DirectX::DX11##BaseName DXObject;\
bool isValid = false;

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