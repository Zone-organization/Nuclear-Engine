#pragma once
#include <NE_Common.h>
#include <API\Color.h>
#include <Math\Matrix.h>
#include <Core\Renderer3D.h>

namespace NuclearEngine {

	namespace Components {

		namespace Internal {
			struct NEAPI _DirLight
			{
				Math::Vector4 m_dir;
				Math::Vector4 m_color;
			};

			struct NEAPI _PointLight
			{
				Math::Vector4 m_pos;
				Math::Vector4 m_intensity_att;
				Math::Vector4 m_color;
			};

			struct NEAPI _SpotLight
			{
				Math::Vector4 m_pos;
				Math::Vector4 m_dir;
				Math::Vector4 m_intensity_att;
				Math::Vector4 m_cutoff_outercutoff;
				Math::Vector4 m_color;
			};
		}

		struct NEAPI DirectionalLight
		{
			friend class Core::Renderer3D;
		public:
			DirectionalLight();
			~DirectionalLight();

			void SetColor(API::Color color);

			void SetDirection(Math::Vector3 dir);

		protected:
			Internal::_DirLight data;
		};

		struct NEAPI PointLight
		{
			friend class Core::Renderer3D;
		public:
			PointLight();
			~PointLight();

			void SetColor(API::Color color);

			void SetPosition(Math::Vector3 pos);

			void SetIntensity(float intensity);

			void SetAttenuation(Math::Vector3 att);

		protected:
			Internal::_PointLight data;
		};

		struct NEAPI SpotLight
		{
			friend class Core::Renderer3D;
		public:
			SpotLight();
			~SpotLight();

			void SetColor(API::Color color);

			void SetPosition(Math::Vector3 pos);

			void SetDirection(Math::Vector3 dir);

			void SetIntensity(float intensity);

			void SetAttenuation(Math::Vector3 att);

			void SetSpotlightCone(Math::Vector2 cutoff_outercutoff);

		protected:
			Internal::_SpotLight data;

		};
	}
}