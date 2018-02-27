#pragma once
#include <NE_Common.h>
#include <vector>
namespace NuclearEngine 
{
	namespace Shading
	{

		struct NEAPI Technique_Requirements
		{
			bool Normals = false;
		};

		// Interface
		class NEAPI Technique {
		public:
			Technique();

			virtual std::string GetShaderPath();
			virtual std::vector<std::string> GetDefines();
		protected:
			std::string m_shaderpath = "";
			std::vector<std::string> defines;
		};
	}
}