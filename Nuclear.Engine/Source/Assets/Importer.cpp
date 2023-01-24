#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>
#include <Assets/Importers\TextureImporter.h>
#include <Utilities/Logger.h>
#include <Scripting\ScriptingEngine.h>

#include <ft2build.h>
#include <freetype/freetype.h> 
#include <msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

#include <Fallbacks/FallbacksEngine.h>
#include <Audio\AudioEngine.h>
#include <Platform\FileSystem.h>
#include <Parsers/ShaderParser.h>

#include <FMOD/inc/fmod.hpp>
#include <Threading/Task.h>
#include <Threading/MainThreadTask.h>
#include <Threading/ThreadingEngine.h>

#include <Assets/AssetManager.h>
#include <Assets\AssetLibrary.h>

#include <Assets/Tasks/TextureImportTask.h>

#include <filesystem>

namespace Nuclear
{
	namespace Assets
	{
		Importer::Importer()
		{
			FT_Handle = msdfgen::initializeFreetype();

			if (FT_Handle == nullptr)
			{
				NUCLEAR_ERROR("[Importer] Failed To Initialize FreeType library");
			}
		}
		Importer& Importer::GetInstance()
		{
			static Importer instance;

			return instance;
		}
		void Importer::Test()
		{
			Assets::Model model;
			model.pMesh = new Assets::Mesh();
			model.pAnimations = new Assets::Animations();
			//mAssimpImporter.Import("../Assets/Common/Models/CrytekNanosuit/nanosuit.obj", &model, Assets::ModelImportingDesc());
		
		}

		std::string AssetNameFromDirectory(const std::string& str)
		{
			std::size_t found = str.find_last_of("/\\");
			std::string directory_path = str.substr(0, found);

			std::size_t found2 = directory_path.find_last_of("/\\");
			std::string directory_path_name = directory_path.substr(found2 + 1);

			std::string filename = str.substr(found + 1);

			filename = filename.substr(0, filename.find_last_of('.'));


			return directory_path_name + "_" + filename;
		}

		Texture* Importer::ImportTexture(const Core::Path& Path, const TextureImportingDesc& Desc)
		{
			if (Desc.mCommonOptions.mAsyncImport)
			{
				//Add to queue			
				auto result = &AssetLibrary::GetInstance().mImportedTextures.AddAsset();
				if (Desc.mCommonOptions.mAssetName == "")
				{
					result->SetName(AssetNameFromDirectory(Path.GetRealPath()));
				}

				mQueuedAssets.push_back(result);
				result->SetState(IAsset::State::Queued);
				Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new TextureImportTask(result, Path , Desc));

				result->SetTextureView(Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage()->GetTextureView());
				return result;
			}
			return ImportTextureST(Path, Desc);
		}

		/**Model* Importer::AsyncImportModel(const Core::Path& Path, const ModelImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto result = AssetLibrary::GetInstance().mImportedModels.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Check if exists
			if (desc.LoadMesh)
			{
				result->pMesh = AssetLibrary::GetInstance().mImportedMeshes.GetOrAddAsset(hashedpath);
			}

			if (desc.LoadMaterialData)
			{
				result->pMaterialData = AssetLibrary::GetInstance().mImportedMaterialDatas.GetOrAddAsset(hashedpath);
			}

			if (desc.LoadAnimation)
			{
				result->pAnimations = AssetLibrary::GetInstance().mImportedAnimations.GetOrAddAsset(hashedpath);
			}


			//Add to queue
			result = &(AssetLibrary::GetInstance().mImportedModels.AddAsset(hashedpath));
			mQueuedAssets.push_back(result);
			result->SetState(IAsset::State::Queued);
			//Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new AssimpImportTask(result, { Path, hashedpath, true }, Desc));


			if (!mAssimpImporter.Load(desc, Path.GetRealPath(), result->pMesh, result->pMaterialData, result->pAnimations))
			{
				NUCLEAR_ERROR("[Importer] Failed to import Model : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

				return nullptr;
			}

			if (desc.LoadAnimation)
			{
				if (result->pAnimations->GetState() != IAsset::State::Loaded)
				{
					AssetLibrary::GetInstance().mImportedAnimations.mData.erase(hashedpath);
				}
			}
			FinishImportingAsset(result->pMesh, Path, hashedpath);
			FinishImportingAsset(result->pMaterialData, Path, hashedpath, "", false);
			FinishImportingAsset(result->pAnimations, Path, hashedpath, "", false);

			if (result->pMesh)
				result->pMesh->Create();

			AssetLibrary::GetInstance().mImportedModels.mData[hashedpath] = result;

			return &AssetLibrary::GetInstance().mImportedModels.mData[hashedpath];
		}
		*/
		Texture* Importer::ImportTextureST(const Core::Path& Path, const TextureImportingDesc& importingdesc)
		{
			//Load
			TextureDesc desc;
			TextureData data;

			Importers::TextureImporter::GetInstance().Load(Path.GetRealPath(), &desc, importingdesc);

			auto result = &AssetLibrary::GetInstance().mImportedTextures.AddAsset();
			if (importingdesc.mCommonOptions.mAssetName == "")
			{
				result->SetName(AssetNameFromDirectory(Path.GetRealPath()));
			}

			Graphics::GraphicsEngine::GetInstance().CreateImageData(&data, desc);

			if (!Graphics::GraphicsEngine::GetInstance().CreateImage(result, &data))
			{
				NUCLEAR_ERROR("[Importer] Failed To Load Texture: '{0}'", Path.GetInputPath());
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}


			result->mState = IAsset::State::Loaded;
			NUCLEAR_INFO("[Assets] Imported: {0} ", Path.GetInputPath());

			return result;
		}

		Texture* Importer::ImportTexture(const TextureDesc& imagedesc, const TextureImportingDesc& importingdesc)
		{
			//Create
			auto result = &AssetLibrary::GetInstance().mImportedTextures.AddAsset();

			TextureData imagedata;
			Graphics::GraphicsEngine::GetInstance().CreateImageData(&imagedata, imagedesc);
			
			if (!Graphics::GraphicsEngine::GetInstance().CreateImage(result, &imagedata))
			{
				NUCLEAR_ERROR("[Importer] Failed To Create Texture : '");
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}


			result->mState = IAsset::State::Loaded;
			NUCLEAR_INFO("[Assets] Imported: {0} ", imagedesc.mPath);
			return result;
		}

		AudioClip* Importer::ImportAudioClip(const Core::Path& Path, const AudioClipImportingDesc& Desc)
		{
			auto result = &AssetLibrary::GetInstance().mImportedAudioClips.AddAsset();
			Audio::AudioEngine::GetInstance().GetSystem()->createSound(Path.GetRealPath().c_str(), Desc.mMode, 0, &result->mSound);


			result->mState = IAsset::State::Loaded;
			NUCLEAR_INFO("[Assets] Imported: {0} ", Path.GetInputPath());
			return result;
		}


		Model* Importer::ImportModel(const Core::Path& Path, const ModelImportingDesc& desc)
		{
			auto result = &AssetLibrary::GetInstance().mImportedModels.AddAsset();

			std::string assetname = desc.mCommonOptions.mAssetName;

			if (desc.mCommonOptions.mAssetName == "")
			{
				assetname = AssetNameFromDirectory(Path.GetRealPath());
			}

			result->SetName(assetname);

			if (desc.LoadMesh)
			{
				result->pMesh = &AssetLibrary::GetInstance().mImportedMeshes.AddAsset();
				result->pMesh->SetName(assetname);
			}
			if (desc.LoadAnimation)
			{
				result->pAnimations = &AssetLibrary::GetInstance().mImportedAnimations.AddAsset();
				result->pAnimations->SetName(assetname);
			}
		

			std::string exportpath;

			if (desc.mCommonOptions.mExportPath.isValid())
			{
				exportpath = desc.mCommonOptions.mExportPath.GetRealPath();
			}
			else
			{
				exportpath = AssetLibrary::GetInstance().GetPath() + "Models/" + result->GetName() + '/';
			}

			Platform::FileSystem::GetInstance().CreateFolders(exportpath + "Textures/");
			

			if (!mAssimpImporter.Import(Path.GetRealPath(), exportpath, result, desc))
			{
				NUCLEAR_ERROR("[Importer] Failed to import Model : '{0}'", Path.GetInputPath());

				return nullptr;
			}

			if (desc.LoadAnimation)
			{
				if (result->pAnimations->GetState() != IAsset::State::Loaded)
				{
					AssetLibrary::GetInstance().mImportedAnimations.mData.erase(result->pAnimations->GetUUID());
					result->pAnimations = nullptr;
				}
				else
				{
					result->pAnimations->mState = IAsset::State::Loaded;
				}
			}

			result->mState = IAsset::State::Loaded;
			result->pMesh->mState = IAsset::State::Loaded;

			NUCLEAR_INFO("[Assets] Imported: {0} ", Path.GetInputPath());

			if (result->pMesh)
				result->pMesh->Create();

			return result;
		}

		Material* Importer::ImportMaterial(const Core::Path& Path, const MaterialImportingDesc& desc)
		{
			namespace fs = std::filesystem;
			auto& material = AssetLibrary::GetInstance().mImportedMaterials.AddAsset();

			try
			{
				std::string newpath = AssetLibrary::GetInstance().GetPath() + "Materials/" + Path.GetFilename();
				fs::copy_file(Path.GetRealPath(), newpath);

				//load
				Serialization::BinaryBuffer buffer;
				Platform::FileSystem::GetInstance().LoadBinaryBuffer(buffer, newpath);
				zpp::bits::in in(buffer);
				in(material);

				NUCLEAR_INFO("[Assets] Imported: {0} ", Path.GetInputPath());
			}
			catch (fs::filesystem_error& e)
			{
				NUCLEAR_ERROR("[Importer] Importing Material '{0}' Failed : '{1}'", Path.GetInputPath(), e.what());
			}
			return &material;
		}

		template <int N>
		void fillfontdesc(const msdfgen::BitmapConstRef<Byte, N>& bitmap, FontCreationDesc& desc)
		{
			desc.AtlasWidth = bitmap.width;
			desc.AtlasHeight = bitmap.height;
			desc.Data = bitmap.pixels;
		}
		Font* Importer::ImportFont(const Core::Path& Path, const FontImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			using namespace msdf_atlas;

			if (msdfgen::FontHandle* font = msdfgen::loadFont(FT_Handle, Path.GetRealPath().c_str()))
			{
				// Storage for glyph geometry and their coordinates in the atlas
				std::vector<GlyphGeometry> glyphs;
				// FontGeometry is a helper class that loads a set of glyphs from a single font.
				// It can also be used to get additional font metrics, kerning information, etc.
				FontGeometry fontGeometry(&glyphs);
				// Load a set of character glyphs:
				// The second argument can be ignored unless you mix different font sizes in one atlas.
				// In the last argument, you can specify a charset other than ASCII.
				// To load specific glyph indices, use loadGlyphs instead.
				fontGeometry.loadCharset(font, 1.0, Charset::ASCII);


				// Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
				//const double maxCornerAngle = 3.0;
				//for (GlyphGeometry& glyph : glyphs)
				//	glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);


				// TightAtlasPacker class computes the layout of the atlas.
				TightAtlasPacker packer;
				// Set atlas parameters:
				// setDimensions or setDimensionsConstraint to find the best value
				packer.setDimensionsConstraint(TightAtlasPacker::DimensionsConstraint::SQUARE);
				// setScale for a fixed size or setMinimumScale to use the largest that fits
				packer.setMinimumScale(24.0);
				// setPixelRange or setUnitRange
				packer.setPixelRange(2.0);
				packer.setMiterLimit(1.0);
				// Compute atlas layout - pack glyphs
				packer.pack(glyphs.data(), glyphs.size());
				// Get final atlas dimensions
				int width = 0, height = 0;
				packer.getDimensions(width, height);
				// The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
				ImmediateAtlasGenerator<
					float, // pixel type of buffer for individual glyphs depends on generator function
					1, // number of atlas color channels
					sdfGenerator, // function to generate bitmaps for individual glyphs
					BitmapAtlasStorage<Byte, 1> // class that stores the atlas bitmap
					// For example, a custom atlas storage class that stores it in VRAM can be used.
				> generator(width, height);
				// GeneratorAttributes can be modified to change the generator's default settings.
				GeneratorAttributes attributes;
				generator.setAttributes(attributes);
				generator.setThreadCount(4);
				// Generate atlas bitmap
				generator.generate(glyphs.data(), glyphs.size());
				// The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
				// The glyphs array (or fontGeometry) contains positioning data for typesetting text.


				//	success = myProject::submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
				auto atlas = generator.atlasStorage();

				Font* result = &AssetLibrary::GetInstance().mImportedFonts.AddAsset();

				FontCreationDesc desc;
				fillfontdesc<1>(atlas, desc);

				Uint32 x = 'f';


				for (auto& i : glyphs)
				{
					FontCharacter glyph;
					glyph.CodePoint = i.getCodepoint();
					glyph.Advance = i.getAdvance();
					//		i.getQuadAtlasBounds
					auto x = i.getBoxRect();
					double l, b, r, t;
					i.getQuadAtlasBounds(l, b, r, t);
					result->Characters[i.getCodepoint()] = glyph;

				}
				result->Create(desc);

				NUCLEAR_INFO("[Importer] Imported Font : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

				// Cleanup
				msdfgen::destroyFont(font);
				return result;
			}
			else
			{
				NUCLEAR_ERROR("[Importer] Failed to import font : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
			}

			//TODO

			return nullptr;
		}

		Shader* Importer::ImportShader(const Core::Path& Path, const ShaderImportingDesc& desc)
		{
			Shader* result = &AssetLibrary::GetInstance().mImportedShaders.AddAsset();
			result->SetName(Path.GetFilename(true));

			auto source = Platform::FileSystem::GetInstance().LoadFileToString(Path.GetRealPath());

			//Step1: Parse Source -> Build ShaderBuildDesc
			Graphics::ShaderBuildDesc& shaderbuilddesc = result->mBuildDesc;
			shaderbuilddesc.mType = desc.mType;
			shaderbuilddesc.mDefines = desc.mDefines;
			shaderbuilddesc.mExcludedVariants = desc.mExcludedVariants;
			bool parsing_ = Parsers::ShaderParser::ParseSource(source, shaderbuilddesc);


			if (parsing_)
			{
				//Step2: Reflect Pixel shader for material reflection
				if (Graphics::GraphicsEngine::GetInstance().ReflectShader(shaderbuilddesc, result->mReflection))
				{
					//Step3: Create actual pipeline
					result->mPipeline.Create(shaderbuilddesc.mPipelineDesc);

					//step 4: export shader info
					AssetManager::GetInstance().Export(result, AssetLibrary::GetInstance().GetPath() + "Shaders/");
				}
			}
			else
			{

			}

			result->mState = IAsset::State::Loaded;
			NUCLEAR_INFO("[Assets] Imported: {0} ", Path.GetInputPath());
			return result;
		}

		Script* Importer::ImportScript(const Core::Path& Path, const ScriptImportingDesc& desc)
		{
			Script* result = &AssetLibrary::GetInstance().mImportedScripts.AddAsset();

			std::string fullname = "";
			if (desc.mClassNameFromPath)
			{
				fullname = Scripting::ScriptingEngine::GetInstance().GetClientAssembly()->GetNamespaceName() + '.' + Path.GetFilename(true);
			}
			else
			{
				fullname = desc.mScriptFullName;
			}
			Scripting::ScriptingEngine::GetInstance().CreateScriptAsset(result, fullname);

			result->mState = IAsset::State::Loaded;
			NUCLEAR_INFO("[Assets] Imported: {0} ", Path.GetInputPath());
			return result;
		}

		std::array<Texture*, 6> Importer::ImportTextureCube(const std::array<Core::Path, 6>& Paths, const TextureImportingDesc& desc)
		{
			TextureImportingDesc Desc = desc;
			//Desc.FlipY_Axis = false;

			std::array<Texture*, 6> result = {
				TextureCube_Import(Paths.at(0), Desc),
				TextureCube_Import(Paths.at(1), Desc),
				TextureCube_Import(Paths.at(2), Desc),
				TextureCube_Import(Paths.at(3), Desc),
				TextureCube_Import(Paths.at(4), Desc),
				TextureCube_Import(Paths.at(5), Desc) };

			return result;
		}

		AssetType Importer::GetAssetType(const std::string& filename)
		{
			std::string extension = filename.substr(filename.find_last_of("."));


			if (Importers::TextureImporter::GetInstance().IsExtensionSupported(extension))
			{
				return AssetType::Texture;
			}
			else if (mAssimpImporter.IsExtensionSupported(extension))
			{
				return AssetType::Mesh;
			}
			else if (extension == ".NEShader")
			{
				return AssetType::Shader;
			}
			else if (extension == ".NEMaterial")
			{
				return AssetType::Material;
			}
			else if (extension == ".NEScene")
			{
				return AssetType::Scene;
			}


			return AssetType::Unknown;
		}

		std::vector<IAsset*>& Importer::GetQueuedAssets()
		{
			return mQueuedAssets;
		}

		Scene* Importer::ImportScene(const Core::Path& Path, const SceneImportingDesc& desc)
		{
			Scene* result = &AssetLibrary::GetInstance().mImportedScenes.AddAsset();

			Platform::FileSystem::GetInstance().LoadBinaryBuffer(result->mBinaryBuffer, Path);

			NUCLEAR_INFO("[Importer] Imported Scene : '{0}'", Path.GetInputPath());
			return result;
		}

		Texture* Importer::TextureCube_Import(const Core::Path& Path, const TextureImportingDesc& importingdesc)
		{
			auto result = &AssetLibrary::GetInstance().mImportedTextures.AddAsset();

			TextureDesc imagedesc;
			TextureData imagedata;

			if (!Importers::TextureImporter::GetInstance().Load(Path.GetRealPath(), &imagedesc, importingdesc))
			{
				NUCLEAR_ERROR("[Importer] Failed To Load Texture2D (For CubeMap): '{0}'", Path.GetInputPath());
				return nullptr;
			}
			imagedesc.mGenerateMipMaps = false;
			Graphics::GraphicsEngine::GetInstance().CreateImageData(&imagedata, imagedesc);
			Graphics::GraphicsEngine::GetInstance().CreateImage(result, &imagedata);

			//TODO::
			//result.mData = imagedata;


			NUCLEAR_INFO("[Importer] Imported Texture2D (for CubeMap) : '{0}'", Path.GetInputPath());

			return result;
		}
	}
}