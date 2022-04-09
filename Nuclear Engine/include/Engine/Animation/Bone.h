#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <vector>
#include <list>

namespace NuclearEngine
{
	namespace Animation
	{
		struct KeyPosition
		{
			Math::Vector3 position;
			Float64 timeStamp;
		};

		struct KeyRotation
		{
			Math::Quaternion orientation;
			Float64 timeStamp;
		};

		struct KeyScale
		{
			Math::Vector3 scale;
			Float64 timeStamp;
		};

		struct BoneData
		{
			~BoneData()
			{
				m_Positions.clear();
				m_Rotations.clear();
				m_Scales.clear();
				m_NumPositions = 0;
				m_NumRotations = 0;
				m_NumScalings = 0;
			}
			std::vector<KeyPosition> m_Positions = std::vector<KeyPosition>();
			std::vector<KeyRotation> m_Rotations = std::vector<KeyRotation>();
			std::vector<KeyScale> m_Scales = std::vector<KeyScale>();
			int m_NumPositions = 0;
			int m_NumRotations = 0;
			int m_NumScalings = 0;
		};

		class NEAPI Bone
		{
		public:
			Bone(const std::string& name, int ID);
			Bone(const std::string& name, int ID, BoneData data);
			~Bone();

			void Update(float animationTime);
			Math::Matrix4 GetLocalTransform();
			std::string GetBoneName() const;
			int GetBoneID();



			int GetPositionIndex(float animationTime);

			int GetRotationIndex(float animationTime);

			int GetScaleIndex(float animationTime);

			BoneData mData;

		private:

			float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

			Math::Matrix4 InterpolatePosition(float animationTime);

			Math::Matrix4 InterpolateRotation(float animationTime);

			Math::Matrix4 InterpolateScaling(float animationTime);

			Math::Matrix4 m_LocalTransform;
			std::string m_Name;
			int m_ID;
		};
	}
}