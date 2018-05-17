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
					Cullenabled = false;
				}
				GLRasterizerState::~GLRasterizerState()
				{
				}

				//URGENT: NOT FINSIHED!
				void GLRasterizerState::Create(GLRasterizerState* result, const RasterizerStateDesc& type)
				{
					if (type.CullMode != CullMode::None)
					{
						result->Cullenabled = true;
						if (type.CullMode != CullMode::Front)
						{
							result->Cullfacemode = GL_FRONT;
						}
						else {
							result->Cullfacemode = GL_BACK;
						}

						if (type.FrontCounterClockwise)
						{
							result->Frontfaceorientation = GL_CCW;
						}
						else {
							result->Frontfaceorientation = GL_CW;
						}
					}

					if (type.FillMode == FillMode::Wireframe)
					{
						result->Fillmode = GL_LINE;
					}
					else {
						result->Fillmode = GL_FILL;
					}
				}
				void GLRasterizerState::Delete(GLRasterizerState * result)
				{

				}
				void GLRasterizerState::Bind()
				{
					if (this->Cullenabled)
					{
						glEnable(GL_CULL_FACE);
						glCullFace(Cullfacemode);
						glFrontFace(Frontfaceorientation);
					}
					glPolygonMode(GL_FRONT_AND_BACK, Fillmode);

				}
			}
		}
	}
}

#endif