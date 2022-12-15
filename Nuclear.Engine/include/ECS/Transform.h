#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <bitsery/details/serialization_common.h>
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

			Transform(const Math::Vector3& position = Math::Vector3(0.0f),
				const Math::Vector3& scale = Math::Vector3(1.0f),
				const Math::Quaternion& rotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
			~Transform();

			void SetPosition(const Math::Vector3& position);
			void SetRotation(const Math::Quaternion& rotation);
			void SetRotation(const Math::Vector3& axis, float angle);
			void SetRotation(const Math::Vector3& eular);

			void SetScale(const Math::Vector3& scale);
			void SetScale(float scale);

			Math::Vector3 GetLocalPosition();
			Math::Quaternion GetLocalRotation();
			Math::Vector3 GetLocalRotationEular();
			Math::Vector3 GetLocalScale();

			Math::Vector3 GetWorldPosition();

			Math::Matrix4 GetWorldMatrix();

			void Update();
		protected:
			friend bitsery::Access;

			Math::Vector3 mLocalPosition;
			Math::Quaternion mLocalRotation;
			Math::Vector3 mLocalScale;

			Math::Vector3 mWorldPosition = Math::Vector3(0.0f);

			template<class S> void serialize(S& s)
			{
				s.object(mLocalPosition);
				s.object(mLocalRotation);
				s.object(mLocalScale);
				s.object(mWorldPosition);
			}

		private:
			Math::Matrix4 mWorldMatrix = Math::Matrix4(1.0f);
			Byte mDirty = NONE;
		};
	}
}