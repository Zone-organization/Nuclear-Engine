#pragma once
struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;
struct _MonoMethod;
struct _MonoObject;
struct _MonoClass;
struct _MonoType;
struct _MonoString;
namespace Nuclear
{
	namespace Scripting
	{
		typedef _MonoMethod* ScriptFunction;
	}
}