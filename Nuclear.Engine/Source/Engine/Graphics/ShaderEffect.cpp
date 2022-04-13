#include <Engine\Graphics\ShaderEffect.h>
#include <Core/Utilities/Hash.h>

namespace Nuclear
{
	namespace Graphics
	{
		ShaderEffect::ShaderEffect(const std::string& name, Type type, bool InitialValue)
		{
			mName = name;
			mID = Utilities::Hash(name);
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