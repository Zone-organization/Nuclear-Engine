#pragma once
#include <NE_Common.h>
#include <Math/Math.h>
#include <Graphics/Color.h>

namespace Nuclear
{
	namespace Rendering
	{
		class ShadowPass;
	}
	namespace Components
	{	
		//Shader Structs
		namespace Internal
		{
			struct NEAPI LightData
			{
				Math::Vector4 Position;                  //Point - Spot											3f,null
				Math::Vector4 Direction;			     //Dir - Spot											3f,null
				Math::Vector4 Color_Intensity;	         //All													3f,f
				Math::Vector4 Attenuation_FarPlane;		 //Point - Spot (FarPlane needed only for Point)		3f,f 
				Math::Vector4 InnerCutOf_OuterCutoff;    //Spot													f,f,null,null
			};
		}
		class NEAPI LightComponent
		{
		public:			
			enum class Type {
				Directional, 
				Spot,
				Point
			};

			LightComponent();
			LightComponent(Type type);
			~LightComponent();

			enum class ShadowType
			{
				No_Shadows,                        //Disables baking of shadow map for a light
				Simple_Shadows                     //Dir light no CSM
			};

			Type GetType() const;
			void SetType(Type type);

			Graphics::Color GetColor();
			void SetColor(Graphics::Color color);

			float GetIntensity();
			void SetIntensity(float intensity);

			Math::Vector3 GetAttenuation();
			void SetAttenuation(Math::Vector3 att);

			Math::Vector2 GetSpotlightCone() const;
			void SetSpotlightCone(Math::Vector2 cutoff_outercutoff);

			Math::Vector3 GetDirection() const;
			void SetDirection(Math::Vector3 dir);

			bool mCastShadows = false;
			float GetFarPlane();
			void SetFarPlane(float farplane);

			float GetNearPlane();
			void SetNearPlane(float nearplane);

			float GetFOV();
			void SetFOV(float fov);

			Internal::LightData& GetInternalData();
			Math::Vector3 GetInternalPosition();
			void SetInternalPosition(Math::Vector3 pos);

		protected:
			friend class Rendering::ShadowPass;

			Internal::LightData mData;
			ShadowType mShadowType;
			Type mType;
			float mFov, mNearPlane;

			Math::Matrix4 LightSpace;

			Math::Vector4 _internalPos; //needed for simple shadow "not CSM"
		};

	}
}