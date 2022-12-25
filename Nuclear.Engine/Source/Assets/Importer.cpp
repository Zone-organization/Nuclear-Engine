#include <Assets/Importer.h>
#include <Assets/AssetLibrary.h>
#include <Assets/Importers\FreeImageImporter.h>
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
		Image* Importer::ImportImage(const Core::Path& Path, AssetLibrary* library, const ImageImportingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto result = library->mImportedImages.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Load
			ImageData imagedata = Importers::FreeImageImporter::GetInstance().Load(Path.GetRealPath(), Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[Importer] Failed To Load Image: '{0}' Hash: '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}

			//Create
			result = &(library->mImportedImages.AddAsset(Path, hashedpath) = Image(imagedata, Desc));

			FinishImportingAsset(result,  Path, hashedpath, library->mName);
			return result;
		}

		Image* Importer::ImportImage(const ImageData& imagedata, AssetLibrary* library, const ImageImportingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(imagedata.mPath);

			auto result = library->mImportedImages.GetAsset(hashedpath);
			if (result)
			{
				return result;
			}

			//Create
			Image image(imagedata, Desc);

			if (image.mTextureView == nullptr)
			{
				NUCLEAR_ERROR("[Importer] Failed To Create Image Hash: '{0}'", Utilities::int_to_hex<Uint32>(hashedpath));
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
			}
			image.mData.mData = NULL;

			result = &(library->mImportedImages.AddAsset(hashedpath) = Image(imagedata, Desc));

			FinishImportingAsset(result, imagedata.mPath, hashedpath, library->mName);

			return result;
		}

		Graphics::Texture Importer::ImportTexture(const Core::Path& Path, AssetLibrary* library, const TextureImportingDesc& Desc)
		{
			auto image = ImportImage(Path, library, Desc.mImageDesc);

			Graphics::Texture result;
			result.SetImage(image);
			result.SetUsageType(Desc.mType);

			return result;
		}

		Graphics::Texture Importer::ImportTexture(const ImageData& imagedata, AssetLibrary* library, const TextureImportingDesc& Desc)
		{
			auto image = ImportImage(imagedata, library, Desc.mImageDesc);

			Graphics::Texture result;
			result.SetImage(image);
			result.SetUsageType(Desc.mType);

			return result;
		}

		AudioClip* Importer::ImportAudioClip(const Core::Path& Path, AssetLibrary* library, const AudioClipImportingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			library->mImportedAudioClips.mData[hashedpath] = AudioClip();
			auto result = &library->mImportedAudioClips.mData[hashedpath];
			Audio::AudioEngine::GetInstance().GetSystem()->createSound(Path.GetRealPath().c_str(), Desc.mMode, 0, &result->mSound);

			FinishImportingAsset(result,  Path, hashedpath, library->mName);

			return result;
		}


		Model* Importer::ImportModel(const Core::Path& Path, AssetLibrary* library, const ModelImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());
			Model result;

			//Check if exists
			if (desc.LoadMesh)
			{
				result.pMesh = library->mImportedMeshes.GetAsset(hashedpath);

				if (!result.pMesh)
					result.pMesh = &(library->mImportedMeshes.AddAsset(hashedpath) = Mesh());
			}

			if (desc.LoadMaterialData)
			{
				result.pMaterialData = library->mImportedMaterialDatas.GetAsset(hashedpath);

				if (!result.pMaterialData)
					result.pMaterialData = &(library->mImportedMaterialDatas.AddAsset(hashedpath) = MaterialData());
			}

			if (desc.LoadAnimation)
			{
				result.pAnimations = library->mImportedAnimations.GetAsset(hashedpath);

				if (!result.pAnimations)
					result.pAnimations = &(library->mImportedAnimations.AddAsset(hashedpath) = Animations());
			}

			if (!mAssimpImporter.Load(desc, Path.GetRealPath(), result.pMesh, result.pMaterialData, result.pAnimations))
			{
				NUCLEAR_ERROR("[Importer] Failed to import Model : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

				return nullptr;
			}

			if (desc.LoadAnimation)
			{
				if (result.pAnimations->GetState() != IAsset::State::Loaded)
				{
					library->mImportedAnimations.mData.erase(hashedpath);
				}
			}
			FinishImportingAsset(result.pMesh,  Path, hashedpath, library->mName);
			FinishImportingAsset(result.pMaterialData, Path, hashedpath, "", false);
			FinishImportingAsset(result.pAnimations, Path, hashedpath, "", false);

			if (result.pMesh)
				result.pMesh->Create();

			library->mImportedModels.mData[hashedpath] = result;

			return &library->mImportedModels.mData[hashedpath];
		}

		template <int N>
		void fillfontdesc(const msdfgen::BitmapConstRef<Byte, N>& bitmap, FontCreationDesc& desc)
		{
			desc.AtlasWidth = bitmap.width;
			desc.AtlasHeight = bitmap.height;
			desc.Data = bitmap.pixels;
		}
		Font* Importer::ImportFont(const Core::Path& Path, AssetLibrary* library, const FontImportingDesc& desc)
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

				library->mImportedFonts.SavePath(hashedpath, Path);
				library->mImportedFonts.mData[hashedpath] = Font();
				Font* result = &library->mImportedFonts.mData[hashedpath];

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
			else {
				NUCLEAR_ERROR("[Importer] Failed to import font : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
			}

			//TODO

			return nullptr;
		}

		Shader* Importer::ImportShader(const Core::Path& Path, AssetLibrary* library, const ShaderImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = library->mImportedShaders.mData.find(hashedpath);
			if (it != library->mImportedShaders.mData.end())
			{
				return &it->second;
			}


			Shader* result;
			result = &library->mImportedShaders.mData[hashedpath];
			auto source = Platform::FileSystem::GetInstance().LoadFileToString(Path.GetRealPath());
			ShaderBuildDesc shaderbuilddesc;
			shaderbuilddesc.mType = desc.mType;
			shaderbuilddesc.mDefines = desc.mDefines;
			shaderbuilddesc.mExcludedVariants = desc.mExcludedVariants;
			if (Parsers::ShaderParser::ParseSource(source, shaderbuilddesc))
			{
				result->mPipeline.Create(shaderbuilddesc.mPipelineDesc);
			}

			FinishImportingAsset(result,  Path, hashedpath, library->mName);

			return result;
		}

		Script* Importer::ImportScript(const Core::Path& Path, AssetLibrary* library, const ScriptImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = library->mImportedScripts.mData.find(hashedpath);
			if (it != library->mImportedScripts.mData.end())
			{
				return &it->second;
			}
			Script* result;
			result = &library->mImportedScripts.mData[hashedpath];

			std::string fullname = "";
			if (desc.mClassNameFromPath)
			{
				fullname = Scripting::ScriptingEngine::GetInstance().GetClientAssembly()->GetNamespaceName() + '.' + Path.GetFilename(true);
			}
			else {
				fullname = desc.mScriptFullName;
			}
			Scripting::ScriptingEngine::GetInstance().CreateScriptAsset(result, fullname);
			FinishImportingAsset(result,  Path, hashedpath, library->mName);

			return result;
		}

		std::array<Image*, 6> Importer::ImportTextureCube(const std::array<Core::Path, 6>& Paths, AssetLibrary* library, const ImageImportingDesc& desc)
		{
			ImageImportingDesc Desc = desc;
			//Desc.FlipY_Axis = false;

			std::array<Image*, 6> result = { 
				TextureCube_Import(Paths.at(0),library, Desc),
				TextureCube_Import(Paths.at(1),library, Desc),
				TextureCube_Import(Paths.at(2),library, Desc),
				TextureCube_Import(Paths.at(3),library, Desc),
				TextureCube_Import(Paths.at(4),library, Desc),
				TextureCube_Import(Paths.at(5),library, Desc) };

			return result;
		}

		AssetType Importer::GetAssetType(const std::string& filename)
		{
			std::string extension = filename.substr(filename.find_last_of("."));


			if (Importers::FreeImageImporter::GetInstance().IsExtensionSupported(extension))
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

		Scene* Importer::ImportScene(const Core::Path& Path, AssetLibrary* library, const SceneImportingDesc& desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if exists
			auto it = library->mImportedScenes.mData.find(hashedpath);
			if (it != library->mImportedScenes.mData.end())
			{
				return &it->second;
			}
			Scene* result = &library->mImportedScenes.mData[hashedpath];

			Platform::FileSystem::GetInstance().LoadBinaryBuffer(result->mBinaryBuffer, Path);

			NUCLEAR_INFO("[Importer] Imported Scene : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
			return result;
		}

		Image* Importer::TextureCube_Import(const Core::Path& Path, AssetLibrary* library, const ImageImportingDesc& Desc)
		{
			auto hashedpath = Utilities::Hash(Path.GetInputPath());

			//Check if Texture has been Imported before
			auto it = library->mImportedImages.mData.find(hashedpath);
			if (it != library->mImportedImages.mData.end())
			{
				return &it->second;
			}

			Image result;
			ImageData imagedata = Importers::FreeImageImporter::GetInstance().Load(Path.GetRealPath(), Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[Importer] Failed To Load Texture2D (For CubeMap): '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));
				return nullptr;
			}
			result.mData = imagedata;

			library->mImportedImages.mData[hashedpath] = result;

			NUCLEAR_INFO("[Importer] Imported Texture2D (for CubeMap) : '{0}' : '{1}'", Path.GetInputPath(), Utilities::int_to_hex<Uint32>(hashedpath));

			return &library->mImportedImages.mData[hashedpath];
		}

		void Importer::FinishImportingAsset(IAsset* asset, const Core::Path& path, Uint32 Hashedpath, const std::string& libraryname, bool log)
		{
			if (asset)
			{
				asset->mUUID = Utilities::UUID::CreateNewUUID();
				asset->mPathHash = Hashedpath;
				asset->mState = IAsset::State::Loaded;

				if (log)
				{
					NUCLEAR_INFO("[{0}] Imported: {1} ", libraryname,  path.GetInputPath());
				}
			}
			return;
		}
	}
}