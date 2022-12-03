#include "Engine\Assets\Script.h"
namespace Nuclear
{
	namespace Assets
	{
		Script::Script()
		{

		}
	
		Script::~Script()
		{
		}
		const std::string Script::GetClassName()
		{
			return mClassName;
		}
		const std::string Script::GetFullNameClassName()
		{
			return mFullName;
		}
		const std::string Script::GetNamespaceName()
		{
			return mNamespaceName;
		}
	}
}