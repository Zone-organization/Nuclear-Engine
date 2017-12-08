#pragma once
#include <NE_Common.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <Math\Math.h>
#include <Shading\Material.h>

namespace NuclearEngine
{
	namespace ModelLoader
	{	
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