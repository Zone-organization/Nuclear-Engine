#include "Platform\FileSystem.h"
#include <Utilities/Logger.h>
#include <fstream>
#include <filesystem>

namespace Nuclear
{
	namespace Platform
	{
		FileSystem::FileSystem()
		{

		}

		FileSystem& FileSystem::GetInstance()
		{
			static FileSystem instance;
			return instance;
		}

		bool FileSystem::CreateFolders(const std::string& path)
		{
			if (std::filesystem::exists(path))
			{
				return true;
			}
			std::error_code err;
			if (!std::filesystem::create_directories(path, err))
			{
				NUCLEAR_WARN("[FileSystem] Failed to create folder: {0} Error: {1}", path, err.message());
				return false;
			}

			return true;
		}

		std::string FileSystem::LoadFileToString(const Core::Path& Filepath)
			{
				std::ifstream file(Filepath.GetRealPath(), std::ios::in);
				std::string data = "", line = "";

				if (file.is_open())
				{
					while (std::getline(file, line))
						data += line + "\n";
				}
				else
				{
					NUCLEAR_ERROR("[FileSystem] Couldn't Read File: '{0}'", Filepath.GetRealPath());
					return std::string("");
				}
				return data;
			}

			std::string FileSystem::LoadShader(const Core::Path& Filepath)
			{
				return LoadFileToString(Filepath);
			}

			std::string FileSystem::LoadShader(const Core::Path& Filepath, std::set<std::string> defines, std::set<std::string> includes, bool reverseorder)
			{
				std::ifstream file(Filepath.GetRealPath(), std::ios::in);
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
					NUCLEAR_ERROR("[FileSystem] Couldn't Read Shader File: '{0}'", Filepath.GetRealPath());
					return std::string();
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
						for (auto& i : defines)
						{
							std::string define = "#define " + i + "\n";
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
						for (auto& i : includes)
						{
							std::string define = "#include \"" + i + "\"\n";
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
						for (auto& i : includes)
						{
							std::string define = "#include \"" + i + "\"\n";
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
						for (auto& i : defines)
						{
							std::string define = "#define " + i + "\n";
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

			Utilities::DataBlob FileSystem::LoadFile(const Core::Path& Filepath)
			{
				std::ifstream input(Filepath.GetRealPath(), std::ios::binary);

				return Utilities::DataBlob(std::vector<Uint8>(std::istreambuf_iterator<char>(input), {}));
			}

			bool FileSystem::SaveFile(const Utilities::DataBlob& data, const Core::Path& Path)
			{
				std::ofstream fout(Path.GetRealPath(), std::ios::out | std::ios::binary);
				fout.write((char*)data.GetBuffer().data(), data.GetBuffer().size());
				fout.close();
				return true;
			}

			//Todo: Errors and exceptions
			bool FileSystem::LoadBinaryBuffer(std::vector<Uint8>& buffer, const Core::Path& Filepath)
			{
				std::ifstream input(Filepath.GetRealPath(), std::ios::binary);
				buffer = std::vector<Uint8>(std::istreambuf_iterator<char>(input), {});

				return true;
			}
			bool FileSystem::SaveBinaryBuffer(const std::vector<Uint8>& buffer, const Core::Path& Path)
			{
				std::ofstream fout(Path.GetRealPath(), std::ios::out | std::ios::binary);
				fout.write((char*)buffer.data(), buffer.size());
				fout.close();

				return true;
			}
			bool FileSystem::SaveBinaryBuffer(const std::vector<Uint8>& buffer, const std::string& Filepath)
			{
				std::ofstream fout(Filepath, std::ios::out | std::ios::binary);
				fout.write((char*)buffer.data(), buffer.size());
				fout.close();

				return true;
			}
	}
}