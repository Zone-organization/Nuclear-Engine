#pragma once
#include <string>
#include <Base\BuildConfig.h>
#include <unordered_map>
#include <Engine/Graphics/API/Texture_Types.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			enum class ShaderType {
				Vertex,
				Geometry,
				Pixel
			};

			enum class ShaderVariableType
			{
				Unknown,
				Bool,
				Int1,
				Int2,
				Int3,
				Int4,
				Float1,
				Float2,	//Vector2
				Float3, //Vector3
				Float4, //Vector4
				Matrix2x2,
				Matrix3x3,
				Matrix4x4,
				Struct
			};
			struct ShaderVariable
			{
				unsigned int Size = 4;           // Size of variable (in bytes)
				ShaderVariableType Type = ShaderVariableType::Float1;
			};
			struct Reflected_Constantbuffer
			{
				unsigned int BindingSlot = 0;
				unsigned int Size = 0;
				std::unordered_map<std::string, ShaderVariable> Variables;
			};
			struct Reflected_Texture
			{
				unsigned int BindingSlot = 0;
				TextureType Type = TextureType::Texture2D;
			};

			struct ShaderReflection
			{
				std::unordered_map<std::string, Reflected_Constantbuffer> ConstantBuffers;
				std::unordered_map<std::string, Reflected_Texture> Textures;
				//Let me clarify certain things here
				//In OpenGL, XShaderCompiler changes HLSL 2 GLSL with limited reflection data as names and locations only!
				//For materials we need to know the texture type and constant buffer contentsso we query these information when creating shaders at runtime 
				//but in directx all these information are available at compile time
				bool Usable = false;
			};

#ifdef NE_COMPILE_D3DCOMPILER
			struct DXBC_BLOB
			{
				void* Buffer = nullptr;
				size_t Size = 0;
			};
#endif

			struct BinaryShaderBlob
			{
#ifdef NE_COMPILE_D3DCOMPILER
				DXBC_BLOB DXBC_SourceCode = DXBC_BLOB();
#endif
#ifdef NE_COMPILE_CORE_OPENGL
				std::string GLSL_SourceCode;
				bool convertedshaderrowmajor = false;
#endif
				ShaderReflection Reflection;

				//If this blob is crosscompiled (as HLSL 2 GLSL)
				bool Converted = false;

				//A usable shader??
				bool Finished = false;
			};
		}
	}
}