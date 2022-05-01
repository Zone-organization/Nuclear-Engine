#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <cereal\cereal.hpp>
namespace Nuclear
{
	namespace ECS
	{
		// an entity transform
		class NEAPI Transform
		{
		public:
			enum TransformChanged : Byte
			{
				NONE = 0,
				TRANSLATION = 1,
				ROTATION = 2,
				SCALE = 4,
				ALL = 7
			};

			Transform(Math::Vector3 position = Math::Vector3(0.0f), Math::Vector3 scale = Math::Vector3(1.0f), Math::Quaternion rotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
			~Transform();

			void SetPosition(Math::Vector3 position);
			void SetRotation(Math::Quaternion rotation);
			void SetRotation(const Math::Vector3& axis, float angle);
			void SetRotation(const Math::Vector3& eular);

			void SetScale(Math::Vector3 scale);
			void SetScale(float scale);

			Math::Vector3 GetLocalPosition();
			Math::Quaternion GetLocalRotation();
			Math::Vector3 GetLocalRotationEular();
			Math::Vector3 GetLocalScale();

			Math::Vector3 GetWorldPosition();

			Math::Matrix4 GetWorldMatrix();

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(CEREAL_NVP(mLocalPosition));
				archive(CEREAL_NVP(mLocalRotation));
				archive(CEREAL_NVP(mLocalScale));
				archive(CEREAL_NVP(mWorldPosition));
			}

			void Update();
		private:

			Math::Vector3 mLocalPosition;
			Math::Quaternion mLocalRotation;
			Math::Vector3 mLocalScale;

			Math::Vector3 mWorldPosition = Math::Vector3(0.0f);

			Math::Matrix4 mWorldMatrix = Math::Matrix4(1.0f);
			Byte mDirty = NONE;
		};
	}
}