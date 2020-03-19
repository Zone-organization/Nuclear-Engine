#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine/Assets/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine/Assets/Common.h>
#include <vector>
#include <string>

namespace NuclearEngine {
	namespace Assets {
		
		struct MeshVertexDesc
		{
			bool TexCoord = true;
			bool Normals = true;
			bool Tangents = true;
		};

		class NEAPI Mesh : public Asset<Mesh>
		{
		public:			
			Mesh();
			~Mesh();
			
			void Create();
			void Release();

			static void CreateCube(Mesh* model, const MeshVertexDesc& desc = MeshVertexDesc(), float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(Mesh* model, const MeshVertexDesc& desc = MeshVertexDesc(), float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);
			static void CreatePlane(Mesh* model, const MeshVertexDesc& desc = MeshVertexDesc(), float width = 5.0f, float depth = 5.0f);
			static void CreateGrid(Mesh* model, const MeshVertexDesc& desc = MeshVertexDesc(), float width = 10.0f, float depth = 10.0f, unsigned int m = 10, unsigned int n = 10);
			static void CreateScreenQuad(Mesh* model);

			struct SubMesh {
				struct SubMeshData
				{
					std::vector<Math::Vector3> Positions = std::vector<Math::Vector3>{};
					std::vector<Math::Vector2> UV = std::vector<Math::Vector2>{};
					std::vector<Math::Vector3> Normals = std::vector<Math::Vector3>{};
					std::vector<Math::Vector3> Tangents = std::vector<Math::Vector3>{};

					std::vector<unsigned int> indices = std::vector<unsigned int>{};
					Uint32 TexSetIndex = 0;
				};

				/*  Mesh Data  */
				SubMesh(SubMeshData data);
				SubMesh(const SubMesh& obj);
				~SubMesh();

				void Create();
				void Release();

				SubMeshData data;

				Uint32 mIndicesCount = 0;
				Uint32 mIndicesOffset = 0;

				//Buffers
				RefCntAutoPtr<IBuffer> mIB;
				RefCntAutoPtr<IBuffer> mVB;
			};

			std::vector<SubMesh> mSubMeshes;
		};

	}
}