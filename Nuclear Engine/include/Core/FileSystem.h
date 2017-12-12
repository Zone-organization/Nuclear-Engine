#pragma once
#include "NE_Common.h"
#include <vector>
namespace NuclearEngine
{
	namespace Core
	{
			class NEAPI BinaryBlob {
			public:
				void* GetBufferPointer();

				size_t GetBufferSize();

				void* buffer;
				size_t size;
			};

			class NEAPI FileSystem
			{
			public:
				static std::string LoadFileToString(std::string Filepath);

				static std::string LoadShader(std::string Filepath, std::vector<std::string> defines, std::vector<std::string> includes);

				static char* LoadBinaryFile(std::string Filepath, size_t* size);
			};

	}
}