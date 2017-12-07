#pragma once
#include <NE_Common.h>
#include <API\Color.h>
#include <Math\Math.h>
#include <API\Texture.h>

namespace NuclearEngine {

	namespace Internal 
	{
		struct Shader_Material_Struct
		{
			Math::Vector4  Emissive;
			Math::Vector4  Ambient;
			Math::Vector4  Diffuse;
			Math::Vector4  Specular;
			Math::Vector4  SpecularPower_UseTexture;
		};
	}

	class NEAPI Material
	{
	public:
		void SetEmissive(API::Color Emissive);
		void SetAmbient(API::Color  Ambient);
		void SetDiffuse(API::Color  Diffuse);
		void SetSpecular(API::Color);
		void SetSpecularPower(float power);
		void UseTexture(bool value);

		API::Texture *Diffuse = nullptr;
		API::Texture *Specular = nullptr;

	protected:
		Internal::Shader_Material_Struct data;
	};
}