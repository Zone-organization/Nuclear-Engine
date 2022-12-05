#pragma once
struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;
struct _MonoMethod;
struct _MonoObject;
struct _MonoClass;

namespace Nuclear
{
	namespace Scripting
	{
		typedef _MonoMethod* ScriptFunction;
	}
}