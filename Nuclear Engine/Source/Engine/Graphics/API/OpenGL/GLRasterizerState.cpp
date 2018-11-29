#include <Engine/Graphics/API/OpenGL\GLRasterizerState.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/RenderStates_Types.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{

				GLRasterizerState::GLRasterizerState()
				{
					mCullEnabled = false;
				}
				GLRasterizerState::~GLRasterizerState()
				{
				}

				//URGENT: NOT FINSIHED!
				void GLRasterizerState::Create(GLRasterizerState* result, const RasterizerStateDesc& type)
				{
					if (type.CullMode != CullMode::None)
					{
						result->mCullEnabled = true;
						if (type.CullMode != CullMode::Front)
						{
							result->mCullFaceMode = GL_FRONT;
						}
						else {
							result->mCullFaceMode = GL_BACK;
						}

						if (type.FrontCounterClockwise)
						{
							result->mFrontFaceOrientation = GL_CCW;
						}
						else {
							result->mFrontFaceOrientation = GL_CW;
						}
					}

					if (type.FillMode == FillMode::Wireframe)
					{
						result->mFillMode = GL_LINE;
					}
					else {
						result->mFillMode = GL_FILL;
					}
				}
				void GLRasterizerState::Delete(GLRasterizerState * result)
				{

				}
				void GLRasterizerState::Bind()
				{
					if (this->mCullEnabled)
					{
						glEnable(GL_CULL_FACE);
						glCullFace(mCullFaceMode);
						glFrontFace(mFrontFaceOrientation);
					}
					glPolygonMode(GL_FRONT_AND_BACK, mFillMode);

				}
			}
		}
	}
}

#endif