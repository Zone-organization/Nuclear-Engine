#include "Core\FileSystem.h"
#include <Base\NE_Common.h>
#include <fstream>

namespace NuclearEngine
{
	namespace Core
	{
	
			std::string FileSystem::LoadFileToString(const Path& Filepath)
			{
				std::ifstream file(Filepath.mRealPath, std::ios::in);
				std::string data = "", line = "";

				if (file.is_open())
				{
					while (std::getline(file, line))
						data += line + "\n";
				}
				else
				{
					Log.Error("[FileSystem] Couldn't Read File: " + Filepath.mRealPath + "\n");
					return std::string("NoString");
				}
				return data;
			}

			std::string FileSystem::LoadShader(const Path& Filepath, std::vector<std::string> defines, std::vector<std::string> includes, bool reverseorder)
			{
				std::ifstream file(Filepath.mRealPath, std::ios::in);
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
					Log.Error("[FileSystem] Couldn't Read Shader File: " + Filepath.mRealPath + "\n");
					return std::string("NoString");
				}
				if (reverseorder == false)
				{
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
				}
				else {
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
				}
				return data;
			}		
	}
}