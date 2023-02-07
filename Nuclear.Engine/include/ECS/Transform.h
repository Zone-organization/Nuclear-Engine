#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Serialization/MathSerialization.h>
#include <Serialization/Access.h>

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


			FORCE_INLINE Math::Vector3 GetLocalPosition() const
			{
				return mLocalPosition;
			}

			FORCE_INLINE Math::Quaternion GetLocalRotation() const
			{
				return mLocalRotation;
			}

			FORCE_INLINE Math::Vector3 GetLocalRotationEular() const
			{
				return glm::eulerAngles(mLocalRotation);
			}

			FORCE_INLINE Math::Vector3 GetLocalScale() const
			{
				return mLocalScale;
			}

			FORCE_INLINE Math::Vector3 GetWorldPosition() const
			{
				return mWorldPosition;
			}

			FORCE_INLINE Math::Matrix4 GetWorldMatrix()  const
			{
				return mWorldMatrix;
			}


			void Update();

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mLocalPosition, self.mLocalRotation, self.mLocalScale, self.mWorldPosition);
			}
		protected:
			friend Serialization::Access;

			Math::Vector3 mLocalPosition;
			Math::Quaternion mLocalRotation;
			Math::Vector3 mLocalScale;

			Math::Vector3 mWorldPosition = Math::Vector3(0.0f);

		private:
			Math::Matrix4 mWorldMatrix = Math::Matrix4(1.0f);
			Byte mDirty = NONE;
		};
	}
}