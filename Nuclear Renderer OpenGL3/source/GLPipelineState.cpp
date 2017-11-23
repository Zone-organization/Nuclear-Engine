#include <NuclearCommon\Common_API_Types.h>
#include "..\include\NuclearRendererOGL3\GLPipelineState.h"

using namespace NuclearEngine;

namespace NuclearRenderer {
	GLenum GetGLComparisonFunc(Comparison_Func func)
	{
		switch (func)
		{
		case Comparison_Func::NEVER: return GL_NEVER;
		case Comparison_Func::LESS: return GL_LESS;
		case Comparison_Func::EQUAL: return GL_EQUAL;
		case Comparison_Func::LESS_EQUAL: return GL_LEQUAL;
		case Comparison_Func::GREATER: return GL_GREATER;
		case Comparison_Func::NOT_EQUAL: return GL_NOTEQUAL;
		case Comparison_Func::GREATER_EQUAL: return GL_GEQUAL;
		case Comparison_Func::ALWAYS: return GL_ALWAYS;

		default: return GL_ALWAYS;
		}
	}

	GLPipelineState::GLPipelineState()
	{
	}
	void GLPipelineState::Create(NuclearEngine::PipelineStateDesc type)
	{
		this->desc = &type;
		if (desc->DepthStencilEnabled == true)
		{
			depthmask = this->desc->DepthMaskEnabled;
			depthfunc = GetGLComparisonFunc(this->desc->DepthFunc);

		}
	}
	void GLPipelineState::Delete()
	{
	}
	void GLPipelineState::Bind_DepthStencil()
	{
		//glDepthMask(depthmask);
		glDepthFunc(depthfunc);

		//glEnable(GL_STENCIL_TEST);
		//glStencilMask(facesmask);

	}
	void GLPipelineState::Unbind_DepthStencil()
	{
		//glDepthMask(GL_FALSE);
		glDepthFunc(GL_LESS);
	}
}