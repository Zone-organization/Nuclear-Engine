#pragma once
#include <NE_Common.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <Math\Math.h>

namespace NuclearEngine
{
	namespace ModelLoader
	{
		// Structure: Vertex
		//
		// Description: Model Vertex object that holds
		//	a Position, Normal, and Texture Coordinate
		struct Vertex
		{
			// Position Math::Vector
			Math::Vector3 Position;

			// Normal Math::Vector
			Math::Vector3 Normal;

			// Texture Coordinate Math::Vector
			Math::Vector2 TextureCoordinate;
		};

		struct Material
		{
			Material()
			{
				name;
				Ns = 0.0f;
				Ni = 0.0f;
				d = 0.0f;
				illum = 0;
			}

			// Material Name
			std::string name;
			// Ambient Color
			Math::Vector3 Ka;
			// Diffuse Color
			Math::Vector3 Kd;
			// Specular Color
			Math::Vector3 Ks;
			// Specular Exponent
			float Ns;
			// Optical Density
			float Ni;
			// Dissolve
			float d;
			// Illumination
			int illum;
			// Ambient Texture Map
			std::string map_Ka;
			// Diffuse Texture Map
			std::string map_Kd;
			// Specular Texture Map
			std::string map_Ks;
			// Specular Hightlight Map
			std::string map_Ns;
			// Alpha Texture Map
			std::string map_d;
			// Bump Map
			std::string map_bump;
		};

		// Structure: Mesh
		//
		// Description: A Simple Mesh Object that holds
		//	a name, a vertex list, and an index list
		struct Mesh
		{
			// Default Constructor
			Mesh()
			{

			}
			// Variable Set Constructor
			Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
			{
				Vertices = _Vertices;
				Indices = _Indices;
			}
			// Mesh Name
			std::string MeshName;
			// Vertex List
			std::vector<Vertex> Vertices;
			// Index List
			std::vector<unsigned int> Indices;

			// Material
			Material MeshMaterial;
		};

		// Class: Loader
		//
		// Description: The OBJ Model Loader
		class NEAPI OBJ_Loader
		{
		public:
			// Default Constructor
			OBJ_Loader();
			~OBJ_Loader();

			// Load a file into the loader
			//
			// If file is loaded return true
			//
			// If the file is unable to be found
			// or unable to be loaded return false
			bool LoadFile(std::string Path);

			// Loaded Mesh Objects
			std::vector<Mesh> LoadedMeshes;
			
		private:
			// Generate vertices from a list of positions, 
			//	tcoords, normals and a face line
			void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
				const std::vector<Math::Vector3>& iPositions,
				const std::vector<Math::Vector2>& iTCoords,
				const std::vector<Math::Vector3>& iNormals,
				std::string icurline);

			// Triangulate a list of vertices into a face by printing
			//	inducies corresponding with triangles within it
			void VertexTriangluation(std::vector<unsigned int>& oIndices,
				const std::vector<Vertex>& iVerts);

			// Load Materials from .mtl file
			bool LoadMaterials(std::string path);

			// Loaded Material Objects
			std::vector<Material> LoadedMaterials;
		};
	}
}