#include "Core\FileSystem.h"
#include <fstream>

namespace NuclearEngine
{
	namespace Core
	{
	
			std::string FileSystem::LoadFileToString(std::string Filepath)
			{
				std::ifstream file(Filepath, std::ios::in);
				std::string data = "", line = "";

				if (file.is_open())
				{
					while (std::getline(file, line))
						data += line + "\n";
				}
				else
				{
					Log->Error("[FileSystem] Couldn't Read File: " + Filepath + "\n");
					return std::string("NoString");
					//throw std::runtime_error(std::string("[FileSystem] Couldn't Read File: ") + Filepath);
				}
				return data;
			}

			std::string FileSystem::LoadShader(std::string Filepath, std::vector<std::string> defines, std::vector<std::string> includes)
			{
				std::ifstream file(Filepath, std::ios::in);
				std::string data = "", line = "";

				if (file.is_open())
				{
					while (std::getline(file, line))
					{
						data += line + "\n";
					}
				}
				else
				{
					Log->Error("[FileSystem] Couldn't Read File: " + Filepath);
					return std::string("NoString");
				}

				if (defines.size() > 0)
				{
					std::vector<std::string> MergedCode;

					std::string firstLine = data.substr(0, data.find("\n"));
					//If GLSL
					if (firstLine.find("#version") != std::string::npos)
					{
						data = data.substr(data.find("\n") + 1, data.length() - 1);
						MergedCode.push_back(firstLine + "\n");
					}
					for (unsigned int i = 0; i < defines.size(); ++i)
					{
						std::string define = "#define " + defines[i] + "\n";
						MergedCode.push_back(define);
					}

					MergedCode.push_back(data);
					std::string str;
					for (unsigned int i = 0; i < MergedCode.size(); ++i)
						str = str + MergedCode[i].c_str();

					data = str;
				}
				if (includes.size() > 0)
				{
					std::vector<std::string> MergedCode;

					std::string firstLine = data.substr(0, data.find("\n"));
					//If GLSL
					if (firstLine.find("#version") != std::string::npos)
					{
						data = data.substr(data.find("\n") + 1, data.length() - 1);
						MergedCode.push_back(firstLine + "\n");
					}
					for (unsigned int i = 0; i < includes.size(); ++i)
					{
						std::string define = "#include \"" + includes[i] + "\"\n";
						MergedCode.push_back(define);
					}

					MergedCode.push_back(data);
					std::string str;
					for (unsigned int i = 0; i < MergedCode.size(); ++i)
						str = str + MergedCode[i].c_str();

					data = str;
				}
				return data;
			}


			char* FileSystem::LoadBinaryFile(std::string Filepath, size_t* size)
			{
				std::ifstream stream;
				char* data;

				stream.open(Filepath, std::ifstream::in | std::ifstream::binary);
				if (stream.good())
				{
					stream.seekg(0, std::ios::end);
					*size = size_t(stream.tellg());
					data = new char[*size];
					stream.seekg(0, std::ios::beg);
					stream.read(&data[0], *size);
					stream.close();
				}
				else
				{
					Log->Error("[FileSystem] Couldn't Read File: " + Filepath);
					throw std::runtime_error(std::string("[FileSystem] Couldn't Read File: ") + Filepath);
				}

				return data;
			}

			void * BinaryBlob::GetBufferPointer()
			{
				return buffer;
			}

			size_t BinaryBlob::GetBufferSize()
			{
				return size;
			}

	}
}