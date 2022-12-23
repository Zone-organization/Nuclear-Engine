#include <Assets\AssetManager.h>
#include <Assets\Mesh.h>
#include <Assets\Material.h>
#include <Audio\AudioEngine.h>
#include <Graphics\Context.h>
#include <Utilities/Logger.h>

#include <Scripting\ScriptingEngine.h>
#include <Importers\Internal\FreeImageImporter.h>
#include <utility>
#include <Platform\FileSystem.h>

#include <sstream>

#include <ft2build.h>
#include <freetype/freetype.h> 
#include <msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

#include <Parsers/ShaderParser.h>
#include <Serialization/SerializationEngine.h>

#include <Fallbacks/FallbacksEngine.h>

namespace Nuclear
{
	namespace Assets
	{
	//	AssetManager::AssetManager(AssetManagerDesc desc)
	//		: mDesc(desc), mThreadPool(4)
	//	{
			//mImageImporter = ImageImporterDelegate::create<&FreeImageLoad>();
	//		//mMeshImporter = MeshImporterDelegate::create<&AssimpLoadMesh>();
	//		FT_Handle = nullptr;
	//	}

		void AssetManager::FinishImportingAsset(Assets::IAsset* asset, const Core::Path& path, Uint32 Hashedpath, bool log)
		{
			if (asset)
			{
				asset->mUUID = Utilities::UUID::CreateNewUUID();
				asset->mPathHash = Hashedpath;
				asset->mState = Assets::IAsset::State::Loaded;

				if (log)
				{
					NUCLEAR_INFO("[AssetManager] Imported: {0} ", path.GetInputPath());
				}
			}
			return;
		}

		//AssetManager::~AssetManager()
		//{
		//	FlushContainers(mDesc.mFlushFlagOnShutdown);
		//	msdfgen::deinitializeFreetype(FT_Handle);
		//}
		AssetManager::AssetManager()
		{
			
		}
		AssetManager& AssetManager::GetInstance()
		{
			static AssetManager instance;

			return instance;
		}

		void AssetManager::FlushContainers()
		{
			/*for (auto& it : mLibrary.mImportedMaterials) {
				 Do stuff

				for (auto& it1 : it.second.mPixelShaderTextures) {
					for (auto& it2 : it1.mData) {
						it2.mTex.mTextureView.RawPtr()->Release

						it2.mTex.mTextureView.Release();
					}
				}
				it.second.mTextureView.Release();
			}*/
		//
			mLibrary.mImportedMeshes.Release();

			mLibrary.mImportedImages.Release();

			mLibrary.mImportedMaterials.Release();

			mLibrary.mImportedAnimations.Release();

			mLibrary.mImportedAudioClips.Release();
		}

		void AssetManager::Initialize(AssetManagerDesc desc)
		{
			mDesc = desc;
			
			FT_Handle = msdfgen::initializeFreetype();

			if (FT_Handle == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager] Failed To Initialize FreeType library");
			}
		}

		IAsset* AssetManager::Import(const Core::Path& Path)
		{
			std::string path(Path.GetPathNoExt() + ".NEMeta");
			AssetMetadata meta;
			Serialization::SerializationEngine::GetInstance().Deserialize(meta, path);

			return Import(Path, meta);
		}

		IAsset* AssetManager::Import(const Core::Path& Path, const AssetMetadata& meta)
		{
			if (meta.mType == AssetType::Image)
			{
				return ImportImage(Path);
			}
			return nullptr;
		}

		Assets::Image* AssetManager::ImportImage(const Core::Path& Path, const ImageLoadingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto result = mLibrary.mImportedImages.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Load
			Assets::ImageData imagedata = Importers::Internal::FreeImageImporter::GetInstance().Load(Path.GetRealPath(), Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager] Failed To Load Image: '{0}' Hash: '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}

			//Create
			result = &(mLibrary.mImportedImages.AddAsset(Path, hashedpath) = Assets::Image(imagedata, Desc));

			FinishImportingAsset(result, Path, hashedpath);
			return result;
		}

		Assets::Image* AssetManager::ImportImage(const Assets::ImageData& imagedata, const ImageLoadingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(imagedata.mPath);

			auto result = mLibrary.mImportedImages.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Create
			Assets::Image image(imagedata, Desc);

			if (image.mTextureView == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager] Failed To Create Image Hash: '{0}'", Utilities::int_to_hex<Uint32>(hashedpath));
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}
			image.mData.mData = NULL;

			result = &(mLibrary.mImportedImages.AddAsset(hashedpath) = Assets::Image(imagedata, Desc));

			FinishImportingAsset(result, imagedata.mPath, hashedpath);

			return result;
		}

		Graphics::Texture AssetManager::ImportTexture(const Core::Path & Path, const TextureLoadingDesc& Desc)
		{
			auto image = ImportImage(Path, Desc.mImageDesc);

			Graphics::Texture result;
			result.SetImage(image);
			result.SetUsageType(Desc.mType);
		
			return result;
		}

		Graphics::Texture AssetManager::ImportTexture(const Assets::ImageData& imagedata, const TextureLoadingDesc& Desc)
		{
			auto image = ImportImage(imagedata, Desc.mImageDesc);

			Graphics::Texture result;
			result.SetImage(image);
			result.SetUsageType(Desc.mType);

			return result;
		}

		Assets::AudioClip* AssetManager::ImportAudioClip(const Core::Path& Path, AUDIO_IMPORT_MODE mode)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			mLibrary.mImportedAudioClips.mData[hashedpath] = Assets::AudioClip();
			auto result = &mLibrary.mImportedAudioClips.mData[hashedpath];
			Audio::AudioEngine::GetInstance().GetSystem()->createSound(Path.GetRealPath().c_str(), mode, 0, &result->mSound);

			FinishImportingAsset(result, Path, hashedpath);

			return result;
		}


		ImportedModel AssetManager::ImportModel(const Core::Path& Path, const ModelLoadingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());
			ImportedModel result;

			//Check if exists
			if (desc.LoadMesh)
			{
				result.pMesh = mLibrary.mImportedMeshes.GetAsset(hashedpath);

				if(!result.pMesh)
					result.pMesh = &(mLibrary.mImportedMeshes.AddAsset(hashedpath) = Assets::Mesh());
			}

			if (desc.LoadMaterialData)
			{
				result.pMaterialData = mLibrary.mImportedMaterialDatas.GetAsset(hashedpath);

				if (!result.pMaterialData)
					result.pMaterialData = &(mLibrary.mImportedMaterialDatas.AddAsset(hashedpath) = Assets::MaterialData());
			}

			if (desc.LoadAnimation)
			{
				result.pAnimations = mLibrary.mImportedAnimations.GetAsset(hashedpath);

				if (!result.pAnimations)
					result.pAnimations = &(mLibrary.mImportedAnimations.AddAsset(hashedpath) = Assets::Animations());
			}
		
			if (!mAssimpImporter.Load(desc, Path.GetRealPath(), result.pMesh, result.pMaterialData, result.pAnimations))
			{
				NUCLEAR_ERROR("[AssetManager] Failed to import Model : '{0}' : '{1}'",  Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

				return result;
			}

			if (desc.LoadAnimation)
			{
				if (result.pAnimations->GetState() != Assets::IAsset::State::Loaded)
				{
					mLibrary.mImportedAnimations.mData.erase(hashedpath);
				}
			}
			FinishImportingAsset(result.pMesh, Path, hashedpath);
			FinishImportingAsset(result.pMaterialData, Path, hashedpath ,false);
			FinishImportingAsset(result.pAnimations, Path, hashedpath, false);

			if(result.pMesh)
				result.pMesh->Create();

			return result;
		}

		template <int N>
		void fillfontdesc(const msdfgen::BitmapConstRef<Byte, N>& bitmap, Assets::FontCreationDesc& desc)
		{
			desc.AtlasWidth = bitmap.width;
			desc.AtlasHeight = bitmap.height;
			desc.Data = bitmap.pixels;
		}
		Assets::Font* AssetManager::ImportFont(const Core::Path& Path, const FontLoadingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			using namespace msdf_atlas;
			
			if (msdfgen::FontHandle* font = msdfgen::loadFont(FT_Handle, Path.GetRealPath().c_str())) {
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
				
				mLibrary.mImportedFonts.SavePath(hashedpath, Path);
				mLibrary.mImportedFonts.mData[hashedpath] = Assets::Font();
				Assets::Font* result = &mLibrary.mImportedFonts.mData[hashedpath];

				Assets::FontCreationDesc desc;
				fillfontdesc<1>(atlas, desc);

				Uint32 x = 'f';


				for (auto& i : glyphs)
				{
					Assets::FontCharacter glyph;
					glyph.CodePoint = i.getCodepoint();
					glyph.Advance = i.getAdvance();
			//		i.getQuadAtlasBounds
					auto x = i.getBoxRect();
					double l, b, r, t;
					i.getQuadAtlasBounds(l, b, r, t);
					result->Characters[i.getCodepoint()] = glyph;

				}
				result->Create(desc);

				NUCLEAR_INFO("[AssetManager] Imported Font : '{0}' : '{1}'",  Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

				// Cleanup
				msdfgen::destroyFont(font);
				return result;
			}
			else {
				NUCLEAR_ERROR("[AssetManager] Failed to import font : '{0}' : '{1}'",  Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
			}

			//TODO

			return nullptr;
		}

		Assets::Shader* AssetManager::ImportShader(const Core::Path& Path, const ShaderLoadingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = mLibrary.mImportedShaders.mData.find(hashedpath);
			if (it != mLibrary.mImportedShaders.mData.end())
			{
				return &it->second;
			}


			Assets::Shader* result;
			result = &mLibrary.mImportedShaders.mData[hashedpath];
			auto source = Platform::FileSystem::GetInstance().LoadFileToString(Path.GetRealPath());
			Assets::ShaderBuildDesc shaderbuilddesc;
			shaderbuilddesc.mType = desc.mType;
			shaderbuilddesc.mDefines = desc.mDefines;
			if (Parsers::ShaderParser::ParseSource(source, shaderbuilddesc))
			{
				result->mPipeline.Create(shaderbuilddesc.mPipelineDesc);
			}

			FinishImportingAsset(result, Path, hashedpath);

			return result;
		}

		Assets::Script* AssetManager::ImportScript(const Core::Path& Path, const ScriptLoadingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = mLibrary.mImportedScripts.mData.find(hashedpath);
			if (it != mLibrary.mImportedScripts.mData.end())
			{
				return &it->second;
			}
			Assets::Script* result;
			result = &mLibrary.mImportedScripts.mData[hashedpath];

			std::string fullname = "";
			if (desc.mClassNameFromPath)
			{				
				fullname = Scripting::ScriptingEngine::GetInstance().GetClientAssembly()->GetNamespaceName() + '.' + Path.GetFilename(true);
			}
			else {
				fullname = desc.mScriptFullName;
			}
			Scripting::ScriptingEngine::GetInstance().CreateScriptAsset(result, fullname);
			FinishImportingAsset(result, Path, hashedpath);

			return result;
		}
	
		Assets::AssetType AssetManager::GetAssetType(const std::string& filename)
		{
			std::string extension = filename.substr(filename.find_last_of("."));


			if (Importers::Internal::FreeImageImporter::GetInstance().IsExtensionSupported(extension))
			{
				return Assets::AssetType::Image;
			}
			else if (mAssimpImporter.IsExtensionSupported(extension))
			{
				return Assets::AssetType::Mesh;
			}
			else if (extension == ".NEShader")
			{
				return Assets::AssetType::Shader;
			}

			return Assets::AssetType::Unknown;
		}

		Assets::SavedScene* AssetManager::ImportScene(const Core::Path& Path, const SceneLoadingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = mLibrary.mImportedScenes.mData.find(hashedpath);
			if (it != mLibrary.mImportedScenes.mData.end())
			{
				return &it->second;
			}
			Assets::SavedScene* result = &mLibrary.mImportedScenes.mData[hashedpath];

			Platform::FileSystem::GetInstance().LoadBinaryBuffer(result->mBinaryBuffer, Path);

			NUCLEAR_INFO("[AssetManager] Imported Scene : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
			return result;
		}

		AssetMetadata AssetManager::CreateMetadata(IAsset* asset)
		{
			AssetMetadata result;

			result.mName = asset->GetName();
			result.mHashedName = Utilities::Hash(asset->GetName());
			result.mHashedPath = asset->GetPathHash();
			result.mUUID = asset->GetUUID();
			result.mType = asset->GetType();

			return result;
		}

		bool AssetManager::Export(const Serialization::BinaryBuffer& asset, const Core::Path& Path)
		{
			Platform::FileSystem::GetInstance().SaveBinaryBuffer(asset, Path);

			return false;
		}

		bool AssetManager::Export(IAsset* asset, const Core::Path& Path)
		{
			if (!asset)
			{
				NUCLEAR_ERROR("[AssetManager] Exporting a null asset! To : '{0}'", Path.GetInputPath());
				return false;
			}


			if (asset->GetType() == AssetType::SerializedScene)
			{
				auto scene = static_cast<SavedScene*>(asset);
				if (!scene->mBinaryBuffer.empty())
				{
					Platform::FileSystem::GetInstance().SaveBinaryBuffer(scene->mBinaryBuffer, Path);
					NUCLEAR_INFO("[AssetManager] Exported Scene '{0}' : '{1}' To: '{2}'", scene->GetName(), Utilities::int_to_hex<Uint32>(scene->GetPathHash()), Path.GetInputPath());
					return true;
				}
			}
			/*else  if (asset->GetType() == AssetType::Image)
			{
				 auto image = static_cast<Image*>(asset);

			}
			*/
			//WIP:
			//Convert all assets to unified types.
			return false;
		}

		Assets::Image* AssetManager::TextureCube_Load(const Core::Path& Path, const ImageLoadingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());
			auto doesitexist = DoesImageExist(hashedpath);
			if (doesitexist != nullptr)
			{
				return doesitexist;
			}

			Assets::Image result;
			Assets::ImageData imagedata = Importers::Internal::FreeImageImporter::GetInstance().Load(Path.GetRealPath(), Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[AssetManager] Failed To Load Texture2D (For CubeMap): '{0}' : '{1}'",  Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
				return nullptr;
			}
			result.mData = imagedata;

			mLibrary.mImportedImages.mData[hashedpath] = result;
			
			NUCLEAR_INFO("[AssetManager] Imported Texture2D (for CubeMap) : '{0}' : '{1}'",  Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

			return &mLibrary.mImportedImages.mData[hashedpath];
		}
		Assets::Image* AssetManager::DoesImageExist(Uint32 hashedpath)
		{
			//Check if Texture has been Imported before
			auto it = mLibrary.mImportedImages.mData.find(hashedpath);
			if (it != mLibrary.mImportedImages.mData.end())
			{
				return &it->second;
			}
			return nullptr;
		}
		std::array<Assets::Image*, 6> AssetManager::LoadTextureCubeFromFile(const std::array<Core::Path, 6>& Paths, const ImageLoadingDesc& desc)
		{
			ImageLoadingDesc Desc = desc;
			//Desc.FlipY_Axis = false;

			std::array<Assets::Image*, 6> result = { TextureCube_Load(Paths.at(0), Desc),
				TextureCube_Load(Paths.at(1), Desc), 
				TextureCube_Load(Paths.at(2), Desc), 
				TextureCube_Load(Paths.at(3), Desc),
				TextureCube_Load(Paths.at(4), Desc),
				TextureCube_Load(Paths.at(5), Desc) };

			return result;
		}
	}
}