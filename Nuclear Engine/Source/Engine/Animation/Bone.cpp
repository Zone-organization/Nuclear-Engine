#define GLM_ENABLE_EXPERIMENTAL
#include "Engine\Animations\Bone.h"
#include "..\Source\Engine\Importers\AssimpGLMHelpers.h"
#include "Base/Math/gtx/quaternion.hpp"
namespace NuclearEngine
{
	namespace Animations
	{
		inline Bone::Bone(const std::string& name, int ID, const BoneData& data)
			:
			m_Name(name),
			m_ID(ID),
			m_LocalTransform(1.0f)
		{
			m_NumPositions = data.m_NumPositions;
			m_Positions = data.m_Positions;
			
			m_NumRotations = data.m_NumRotations;
			m_Rotations = data.m_Rotations;

			m_NumScalings = data.m_NumScalings;
			m_Scales = data.m_Scales;

		}
		inline void Bone::Update(float animationTime)
		{
			Math::Matrix4 translation = InterpolatePosition(animationTime);
			Math::Matrix4 rotation = InterpolateRotation(animationTime);
			Math::Matrix4 scale = InterpolateScaling(animationTime);
			m_LocalTransform = translation * rotation * scale;
		}
		inline Math::Matrix4 Bone::GetLocalTransform() { return m_LocalTransform; }
		inline std::string Bone::GetBoneName() const { return m_Name; }
		inline int Bone::GetBoneID() { return m_ID; }
		inline int Bone::GetPositionIndex(float animationTime)
		{
			for (int index = 0; index < m_NumPositions - 1; ++index)
			{
				if (animationTime < m_Positions[index + 1].timeStamp)
					return index;
			}
			assert(0);
		}
		inline int Bone::GetRotationIndex(float animationTime)
		{
			for (int index = 0; index < m_NumRotations - 1; ++index)
			{
				if (animationTime < m_Rotations[index + 1].timeStamp)
					return index;
			}
			assert(0);
		}
		inline int Bone::GetScaleIndex(float animationTime)
		{
			for (int index = 0; index < m_NumScalings - 1; ++index)
			{
				if (animationTime < m_Scales[index + 1].timeStamp)
					return index;
			}
			assert(0);
		}
		inline float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
		{
			float scaleFactor = 0.0f;
			float midWayLength = animationTime - lastTimeStamp;
			float framesDiff = nextTimeStamp - lastTimeStamp;
			scaleFactor = midWayLength / framesDiff;
			return scaleFactor;
		}
		inline Math::Matrix4 Bone::InterpolatePosition(float animationTime)
		{
			if (1 == m_NumPositions)
				return glm::translate(Math::Matrix4(1.0f), m_Positions[0].position);

			int p0Index = GetPositionIndex(animationTime);
			int p1Index = p0Index + 1;
			float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
				m_Positions[p1Index].timeStamp, animationTime);
			Math::Vector3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position
				, scaleFactor);
			return glm::translate(Math::Matrix4(1.0f), finalPosition);
		}
		inline Math::Matrix4 Bone::InterpolateRotation(float animationTime)
		{
			if (1 == m_NumRotations)
			{
				auto rotation = glm::normalize(m_Rotations[0].orientation);
				return glm::toMat4(rotation);
			}

			int p0Index = GetRotationIndex(animationTime);
			int p1Index = p0Index + 1;
			float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
				m_Rotations[p1Index].timeStamp, animationTime);
			glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation
				, scaleFactor);
			finalRotation = glm::normalize(finalRotation);
			return glm::toMat4(finalRotation);

		}
		inline Math::Matrix4 Bone::InterpolateScaling(float animationTime)
		{
			if (1 == m_NumScalings)
				return glm::scale(Math::Matrix4(1.0f), m_Scales[0].scale);

			int p0Index = GetScaleIndex(animationTime);
			int p1Index = p0Index + 1;
			float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
				m_Scales[p1Index].timeStamp, animationTime);
			Math::Vector3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale
				, scaleFactor);
			return glm::scale(Math::Matrix4(1.0f), finalScale);
		}
	}
}