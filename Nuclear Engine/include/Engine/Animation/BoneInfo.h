#pragma once
#include <Base\Math\Math.h>

namespace NuclearEngine
{
	namespace Animation
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