#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <vector>
#include <list>

namespace NuclearEngine
{
	namespace Animations
	{
		struct KeyPosition
		{
			Math::Vector3 position;
			float timeStamp;
		};

		struct KeyRotation
		{
			Math::Quaternion orientation;
			float timeStamp;
		};

		struct KeyScale
		{
			Math::Vector3 scale;
			float timeStamp;
		};

		struct BoneData
		{
			std::vector<KeyPosition> m_Positions;
			std::vector<KeyRotation> m_Rotations;
			std::vector<KeyScale> m_Scales;
			int m_NumPositions;
			int m_NumRotations;
			int m_NumScalings;
		};

		class NEAPI Bone
		{
		public:
			Bone(const std::string& name, int ID, const BoneData& data);

			void Update(float animationTime);
			Math::Matrix4 GetLocalTransform();
			std::string GetBoneName() const;
			int GetBoneID();



			int GetPositionIndex(float animationTime);

			int GetRotationIndex(float animationTime);

			int GetScaleIndex(float animationTime);


		private:

			float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

			Math::Matrix4 InterpolatePosition(float animationTime);

			Math::Matrix4 InterpolateRotation(float animationTime);

			Math::Matrix4 InterpolateScaling(float animationTime);

			std::vector<KeyPosition> m_Positions;
			std::vector<KeyRotation> m_Rotations;
			std::vector<KeyScale> m_Scales;
			int m_NumPositions;
			int m_NumRotations;
			int m_NumScalings;

			Math::Matrix4 m_LocalTransform;
			std::string m_Name;
			int m_ID;
		};
	}
}