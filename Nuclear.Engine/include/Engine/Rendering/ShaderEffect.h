#pragma once
#include <Core\NE_Common.h>
#include <string>


namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI ShaderEffect
		{
		public:
			enum class Type {
				CameraEffect,
				RenderingEffect,
				CameraAndRenderingEffect
			};
			ShaderEffect();
			ShaderEffect(const std::string& name, Type type, bool InitialValue);
			~ShaderEffect();

			void SetValue(bool value);
			bool GetValue();

			Type GetType();

			std::string GetName();
			Uint32 GetID();

			//bool _InternalEnabled = false;
		private:
			Uint32 mID = 0;
			std::string mName;
			Type mType;
			bool mValue;
		};
	}
}