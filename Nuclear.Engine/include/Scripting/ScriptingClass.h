#pragma once
#include <NE_Common.h>
#include <Scripting/ScriptingObject.h>
#include <string>
namespace Nuclear
{
	namespace Scripting
	{
		struct  ScriptingClassCreationDesc
		{
			ScriptingClassCreationDesc()
				: mFullName(""), mNamespaceName(""), mClassName("")
			{

			}
			ScriptingClassCreationDesc(const std::string& Fullname)
			{
				mFullName = Fullname;

				if (mFullName.find('.') != std::string::npos)
				{
					mNamespaceName = mFullName.substr(0, mFullName.find_last_of('.'));
					mClassName = mFullName.substr(mFullName.find_last_of('.') + 1);
				}
				else
				{
					mClassName = mFullName;
				}
			}

			ScriptingClassCreationDesc(const std::string& namespacename, const std::string& classname)
				: mNamespaceName(namespacename), mClassName(classname)
			{
				mClassName = classname;;
				mNamespaceName = namespacename;
				mFullName = mNamespaceName + '.' + mClassName;
			}


			std::string mFullName;
			std::string mNamespaceName;
			std::string mClassName;
		};

		class NEAPI ScriptingClass
		{
		public:
			ScriptingObject CreateObject();

			// method name only "Load()"
			ScriptFunction GetMethod(const std::string& methodname);

			_MonoClass* GetClassPtr();
		private:
			friend class ScriptingEngine;
			_MonoClass* pClass = nullptr;
			ScriptingClassCreationDesc mDesc;
		};
	}
}
