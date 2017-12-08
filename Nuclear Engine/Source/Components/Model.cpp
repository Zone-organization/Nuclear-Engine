#include "Components\Model.h"
#include <ResourceManager/ResourceManager.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;
namespace NuclearEngine {

	namespace Components {
		/*  Functions   */
		// constructor, expects a filepath to a 3D model.

		Model::Model() 
		{
						
			//loadModel(path);
		}

		// draws the model, and thus all its meshes

		void Model::Draw(API::Shader* shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				meshes[i].Draw(shader);
			}
		}

	}
}