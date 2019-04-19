#include "Engine\Graphics\GraphicsEngine.h"
namespace NuclearEngine
{
	namespace Graphics
	{
		Managers::ShaderManager* GraphicsEngine::pDefaultShaderManager = nullptr;
		static bool ShaderManagerAutoCreated = false;
		bool GraphicsEngine::Initialize(const GraphicsEngineDesc & desc)
		{
			if (desc.DefaultShaderManager == nullptr)
			{
				Log.Info("[GraphicsEngine] DefaultShaderManager is a nullptr, engine will auto-creating a new one.\n");
				pDefaultShaderManager = new Managers::ShaderManager();
				ShaderManagerAutoCreated = true;
			}
			else
				pDefaultShaderManager = desc.DefaultShaderManager;

			return true;
		}
		void GraphicsEngine::Shutdown()
		{
			if (ShaderManagerAutoCreated)
			{
				delete pDefaultShaderManager;
			}
			pDefaultShaderManager = nullptr;
		}
		Managers::ShaderManager * GraphicsEngine::GetShaderManager()
		{
			return pDefaultShaderManager;
		}
	}
}