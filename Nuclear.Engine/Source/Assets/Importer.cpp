#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>
#include <Assets/Importers\ImageImporter.h>
#include <Utilities/Logger.h>
#include <Utilities/Hash.h>
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

#include <Assets/Tasks/ImageImporting.h>

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
			model.pMaterialData = new Assets::MaterialData();
			model.pAnimations = new Assets::Animations();

			mAssimpImporter.Import("../Assets/Common/Models/CrytekNanosuit/nanosuit.obj", "../Assets/Common/Models/CrytekNanosuit/exportednanosuit.glb", &model, Assets::ModelImportingDesc());
		
		}

		Image* Importer::ImportImage(const Core::Path& Path, const ImageImportingDesc& Desc)
		{
			if (Desc.mAsyncImporting)
			{
				auto hashedpath = Utilities::Hash(Path.GetInputPath());

				//Check if exists
				auto result = AssetLibrary::GetInstance().mImportedImages.GetAsset(hashedpath);
				if (result)
				{
					return result;
				}

				//Add to queue			
				result = &(AssetLibrary::GetInstance().mImportedImages.AddAsset(hashedpath));
				mQueuedAssets.push_back(result);
				result->SetState(IAsset::State::Queued);
				Threading::ThreadingEngine::GetInstance().GetThreadPool().AddTask(new ImageImportTask(result, { Path, hashedpath, true }, Desc));

				result->SetTextureView(Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage()->GetTextureView());
				return result;
			}
			return ImportImageST(Path, Desc);
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
		Image* Importer::ImportImageST(const Core::Path& Path, const ImageImportingDesc& importingdesc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto result = AssetLibrary::GetInstance().mImportedImages.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Load
			ImageDesc desc;
			ImageData data;

			Importers::ImageImporter::GetInstance().Load(Path.GetRealPath(),&desc, importingdesc);

			auto& image = AssetLibrary::GetInstance().mImportedImages.AddAsset(hashedpath);

			Graphics::GraphicsEngine::GetInstance().CreateImageData(&data, desc);

			if (!Graphics::GraphicsEngine::GetInstance().CreateImage(&image, &data))
			{
				NUCLEAR_ERROR("[Importer] Failed To Load Image: '{0}' Hash: '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}

			//Create
			result = &image;

			FinishImportingAsset(result, Path, hashedpath);
			return result;
		}

		Image* Importer::ImportImage(const ImageDesc& imagedesc, const ImageImportingDesc& importingdesc)
		{
			auto hashedpath = Utilities::Hash(imagedesc.mPath);

			auto result = AssetLibrary::GetInstance().mImportedImages.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Create
			Image image;
			ImageData imagedata;
			Graphics::GraphicsEngine::GetInstance().CreateImageData(&imagedata, imagedesc);
			
			if (!Graphics::GraphicsEngine::GetInstance().CreateImage(&image, &imagedata))
			{
				NUCLEAR_ERROR("[Importer] Failed To Create Image Hash: '{0}'", Utilities::int_to_hex<Uint32>(hashedpath));
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}

			result = &(AssetLibrary::GetInstance().mImportedImages.AddAsset(hashedpath) = image);

			FinishImportingAsset(result, imagedesc.mPath, hashedpath);

			return result;
		}

		Graphics::Texture Importer::ImportTexture(const Core::Path& Path, const TextureImportingDesc& Desc)
		{
			auto image = ImportImage(Path, Desc.mImageDesc);

			Graphics::Texture result;
			result.SetImage(image);
			result.SetUsageType(Desc.mType);

			return result;
		}

		Graphics::Texture Importer::ImportTexture(const ImageDesc& imagedata, const TextureImportingDesc& Desc)
		{
			auto image = ImportImage(imagedata, Desc.mImageDesc);

			Graphics::Texture result;
			result.SetImage(image);
			result.SetUsageType(Desc.mType);

			return result;
		}

		AudioClip* Importer::ImportAudioClip(const Core::Path& Path, const AudioClipImportingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			AssetLibrary::GetInstance().mImportedAudioClips.mData[hashedpath] = AudioClip();
			auto result = &AssetLibrary::GetInstance().mImportedAudioClips.mData[hashedpath];
			Audio::AudioEngine::GetInstance().GetSystem()->createSound(Path.GetRealPath().c_str(), Desc.mMode, 0, &result->mSound);

			FinishImportingAsset(result, Path, hashedpath);

			return result;
		}


		Model* Importer::ImportModel(const Core::Path& Path, const ModelImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());


			//Check if exists
			auto result = AssetLibrary::GetInstance().mImportedModels.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}
			result = &AssetLibrary::GetInstance().mImportedModels.AddAsset(hashedpath);
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

			if (!mAssimpImporter.Load(Path.GetRealPath(), result, desc))
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
			FinishImportingAsset(result->pMaterialData, Path, hashedpath, false);
			FinishImportingAsset(result->pAnimations, Path, hashedpath, false);

			if (result->pMesh)
				result->pMesh->Create();

			return result;
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

				AssetLibrary::GetInstance().mImportedFonts.SavePath(hashedpath, Path);
				AssetLibrary::GetInstance().mImportedFonts.mData[hashedpath] = Font();
				Font* result = &AssetLibrary::GetInstance().mImportedFonts.mData[hashedpath];

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
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = AssetLibrary::GetInstance().mImportedShaders.mData.find(hashedpath);
			if (it != AssetLibrary::GetInstance().mImportedShaders.mData.end())
			{
				return &it->second;
			}


			Shader* result;
			result = &AssetLibrary::GetInstance().mImportedShaders.mData[hashedpath];
			auto source = Platform::FileSystem::GetInstance().LoadFileToString(Path.GetRealPath());
			ShaderBuildDesc shaderbuilddesc;
			shaderbuilddesc.mType = desc.mType;
			shaderbuilddesc.mDefines = desc.mDefines;
			shaderbuilddesc.mExcludedVariants = desc.mExcludedVariants;
			if (Parsers::ShaderParser::ParseSource(source, shaderbuilddesc))
			{
				result->mPipeline.Create(shaderbuilddesc.mPipelineDesc);
			}

			FinishImportingAsset(result, Path, hashedpath);

			return result;
		}

		Script* Importer::ImportScript(const Core::Path& Path, const ScriptImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = AssetLibrary::GetInstance().mImportedScripts.mData.find(hashedpath);
			if (it != AssetLibrary::GetInstance().mImportedScripts.mData.end())
			{
				return &it->second;
			}
			Script* result;
			result = &AssetLibrary::GetInstance().mImportedScripts.mData[hashedpath];

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
			FinishImportingAsset(result, Path, hashedpath);

			return result;
		}

		std::array<Image*, 6> Importer::ImportTextureCube(const std::array<Core::Path, 6>& Paths, const ImageImportingDesc& desc)
		{
			ImageImportingDesc Desc = desc;
			//Desc.FlipY_Axis = false;

			std::array<Image*, 6> result = {
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


			if (Importers::ImageImporter::GetInstance().IsExtensionSupported(extension))
			{
				return AssetType::Image;
			}
			else if (mAssimpImporter.IsExtensionSupported(extension))
			{
				return AssetType::Mesh;
			}
			else if (extension == ".NEShader")
			{
				return AssetType::Shader;
			}

			return AssetType::Unknown;
		}

		std::vector<IAsset*>& Importer::GetQueuedAssets()
		{
			return mQueuedAssets;
		}

		Scene* Importer::ImportScene(const Core::Path& Path, const SceneImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = AssetLibrary::GetInstance().mImportedScenes.mData.find(hashedpath);
			if (it != AssetLibrary::GetInstance().mImportedScenes.mData.end())
			{
				return &it->second;
			}
			Scene* result = &AssetLibrary::GetInstance().mImportedScenes.mData[hashedpath];

			Platform::FileSystem::GetInstance().LoadBinaryBuffer(result->mBinaryBuffer, Path);

			NUCLEAR_INFO("[Importer] Imported Scene : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
			return result;
		}

		Image* Importer::TextureCube_Import(const Core::Path& Path, const ImageImportingDesc& importingdesc)
		{

			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if Texture has been Imported before
			auto it = AssetLibrary::GetInstance().mImportedImages.mData.find(hashedpath);
			if (it != AssetLibrary::GetInstance().mImportedImages.mData.end())
			{
				return &it->second;
			}

			Image result;
			ImageDesc imagedesc;
			ImageData imagedata;

			if (!Importers::ImageImporter::GetInstance().Load(Path.GetRealPath(), &imagedesc, importingdesc))
			{
				NUCLEAR_ERROR("[Importer] Failed To Load Texture2D (For CubeMap): '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
				return nullptr;
			}
			imagedesc.mGenerateMipMaps = false;
			Graphics::GraphicsEngine::GetInstance().CreateImageData(&imagedata, imagedesc);
			Graphics::GraphicsEngine::GetInstance().CreateImage(&result, &imagedata);

			//TODO::
			//result.mData = imagedata;

			AssetLibrary::GetInstance().mImportedImages.mData[hashedpath] = result;

			NUCLEAR_INFO("[Importer] Imported Texture2D (for CubeMap) : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

			return &AssetLibrary::GetInstance().mImportedImages.mData[hashedpath];
		}

		void Importer::FinishImportingAsset(IAsset* asset, const Core::Path& path, Uint32 Hashedpath, bool log)
		{
			if (asset)
			{
				asset->mPathHash = Hashedpath;
				asset->mState = IAsset::State::Loaded;

				if (log)
				{
					NUCLEAR_INFO("[Assets] Imported: {0} ", path.GetInputPath());
				}
			}
			return;
		}
	}
}