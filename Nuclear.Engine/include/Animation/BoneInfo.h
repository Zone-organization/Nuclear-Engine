#pragma once
#include <Math\Math.h>

namespace Nuclear
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