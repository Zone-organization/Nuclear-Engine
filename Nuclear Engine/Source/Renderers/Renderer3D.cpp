#include <Renderers\Renderer3D.h>
#include <Core\FileSystem.h>

namespace NuclearEngine {
	namespace Renderers {

		namespace Renderer3D_Members
		{
			API::Shader *Shader;
			bool Init = false;
		}

		bool Renderer3D::Initialize()
		{


			return Renderer3D_Members::Init;
		}
		bool Renderer3D::SetTechnique(Shading::Technique * Tech)
		{
			return false;
		}
		void Renderer3D::Reload()
		{
		}
		void Renderer3D::Render_Light()
		{
		}
	}
}