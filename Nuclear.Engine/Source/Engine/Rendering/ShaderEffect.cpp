#include <Engine\Rendering\ShaderEffect.h>
#include <Core/Utilities/Hash.h>

namespace Nuclear
{
	namespace Rendering
	{
		ShaderEffect::ShaderEffect()
		{
		}
		ShaderEffect::ShaderEffect(const std::string& name, Type type, bool InitialValue)
		{
			mName = name;
			mID = Utilities::Hash(mName);
			mValue = InitialValue;
			mType = type;
		}
		ShaderEffect::~ShaderEffect()
		{
		}
		void ShaderEffect::SetValue(bool value)
		{
			mValue = value;
		}
		bool ShaderEffect::GetValue()
		{
			return mValue;
		}
		ShaderEffect::Type ShaderEffect::GetType()
		{
			return mType;
		}
		std::string ShaderEffect::GetName()
		{
			return mName;
		}
		Uint32 ShaderEffect::GetID()
		{
			return mID;
		}
	}
}