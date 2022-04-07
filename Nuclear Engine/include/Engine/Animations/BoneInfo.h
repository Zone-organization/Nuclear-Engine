#pragma once
#include <Base\Math\Math.h>

namespace NuclearEngine
{
	namespace Animations
	{
		struct BoneInfo
		{
			/*id is index in finalBoneMatrices*/
			int id;

			/*offset matrix transforms vertex from model space to bone space*/
			Math::Matrix4 offset;

		};
	}
}