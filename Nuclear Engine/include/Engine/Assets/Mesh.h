#pragma once
#include <Engine/Assets/Common.h>
#include <Engine/Animation/BoneInfo.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace NuclearEngine {
	namespace Assets {
		
		class NEAPI Mesh : public Asset<Mesh>
		{
		public:	
			struct Vertex
			{
				Vertex()
					: Position(Math::Vector3(0.0f)), Normal(Math::Vector3(0.0f)), Tangents(Math::Vector3(0.0f)), UV(Math::Vector2(0.0f)) {}
				Vertex(const Math::Vector3& p, const Math::Vector3& n, const Math::Vector3& t, const Math::Vector2& uv)
					: Position(p), Normal(n), Tangents(t), UV(uv) {}
				Vertex(
					float px, float py, float pz,
					float nx, float ny, float nz,
					float tx, float ty, float tz,
					float u, float v)
					: Position(px, py, pz), Normal(nx, ny, nz),
					Tangents(tx, ty, tz), UV(u, v) {}

				Math::Vector3 Position;
				Math::Vector2 UV;
				Math::Vector3 Normal;
				Math::Vector3 Tangents;
				Math::Vector4i BoneIDs = Math::Vector4i(-1);
				Math::Vector4 Weights = Math::Vector4(0.0f);

			};

			struct SubMesh
			{
				struct SubMeshData
				{

					std::vector<Vertex> Vertices = std::vector<Vertex>();
					std::vector<unsigned int> indices = std::vector<unsigned int>{};
					Uint32 TexSetIndex = 0;
				};

				/*  Mesh Data  */
				SubMesh(SubMeshData data);
				SubMesh(const SubMesh& obj);
				~SubMesh();

				void Create();

				SubMeshData data;

				Uint32 mIndicesCount = 0;
				Uint32 mIndicesOffset = 0;

				//Buffers
				RefCntAutoPtr<IBuffer> mIB;
				RefCntAutoPtr<IBuffer> mVB;
			};

			Mesh(std::vector<SubMesh> SubMeshes, std::unordered_map<std::string, Animation::BoneInfo> BoneInfoMap, int BoneCounter);

			Mesh();
			~Mesh();
			
			void Create();

			static void CreateCube(Mesh* model, float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(Mesh* model, float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);
			static void CreatePlane(Mesh* model, float width = 5.0f, float depth = 5.0f);
			static void CreateGrid(Mesh* model, float width = 10.0f, float depth = 10.0f, unsigned int m = 10, unsigned int n = 10);
			static void CreateScreenQuad(Mesh* model);

			std::unordered_map<std::string, Animation::BoneInfo> mBoneInfoMap;
			int mBoneCounter = 0;
			std::vector<SubMesh> mSubMeshes;
		};

	}
}