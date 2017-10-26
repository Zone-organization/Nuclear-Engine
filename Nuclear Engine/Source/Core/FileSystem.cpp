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

			std::string FileSystem::LoadGLSLShaderWithDefines(std::string Filepath, std::vector<std::string> defines)
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
					return std::string("");

					//throw std::runtime_error(std::string("[FileSystem] Couldn't Read File: ") + Filepath);
				}

				// NOTE(Joey): if a list of define statements is specified, add these 
				// to the start of the shader source, s.t. we can selectively compile
				// different shaders based on the defines we set.
				if (defines.size() > 0)
				{
					std::vector<std::string> MergedCode;
					// NOTE(Joey): first determine if the user supplied a #version 
					// directive at the top of the shader code, in which case we 
					// extract it and add it 'before' the list of define code.
					// The GLSL version specifier is only valid as the first line of
					// the GLSL code; otherwise the GLSL version defaults to 1.1.
					std::string firstLine = data.substr(0, data.find("\n"));
					if (firstLine.find("#version") != std::string::npos)
					{
						// NOTE(Joey): strip shader code of first line and add to list
						// of shader code strings
						data = data.substr(data.find("\n") + 1, data.length() - 1);
						MergedCode.push_back(firstLine + "\n");
					}
					// NOTE(Joey): then add define statements to the shader string list
					for (unsigned int i = 0; i < defines.size(); ++i)
					{
						std::string define = "#define " + defines[i] + "\n";
						MergedCode.push_back(define);
					}
					// then addremaining shader code to merged result and pass result
					// to glShaderSource
					MergedCode.push_back(data);
					// NOTE(Joey): note that we manually build an array of C style 
					// strings as glShaderSource doesn't expect it in any other format.
					// Note that all strings are null-terminated so we can pass NULL
					// as glShaderSource's final argument.
					std::string str;
					for (unsigned int i = 0; i < MergedCode.size(); ++i)
						str = str + MergedCode[i].c_str();

					return str;
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

	}
}