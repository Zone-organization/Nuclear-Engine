#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine\Audio\AudioEngine.h>
#include "Engine\Graphics\GraphicsEngine.h"
#include <Core\Logger.h>


#define EXPOSE_FREEIMAGE_IMPORTER
#define EXPOSE_ASSIMP_IMPORTER
#include <Engine\Importers\AssimpImporter.h>
#include <Engine\Importers\FreeImageImporter.h>
#include <utility>
#include <Core\FileSystem.h>

#include <Engine\ECS\Scene.h>
#include <cereal/archives/json.hpp>
#include <sstream>
#include <Engine\Components\Components.h>

#include <ft2build.h>
#include <freetype/freetype.h> 
#include <msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

namespace Nuclear {
	namespace Managers {

		Graphics::Texture AssetManager::DefaultBlackTex;
		Graphics::Texture AssetManager::DefaultGreyTex;
		Graphics::Texture AssetManager::DefaultWhiteTex;

		Graphics::Texture AssetManager::DefaultDiffuseTex;
		Graphics::Texture AssetManager::DefaultSpecularTex;
		Graphics::Texture AssetManager::DefaultNormalTex;
	
		AssetManager::AssetManager(AssetManagerDesc desc)
			: mDesc(desc)
		{
			mImageImporter = Importers::ImageImporterDelegate::create<&Importers::FreeImageLoad>();
			mMeshImporter = Importers::MeshImporterDelegate::create<&Importers::AssimpLoadMesh>();
			FT_Handle = nullptr;
		}

		AssetManager::~AssetManager()
		{
		//	FlushContainers(mDesc.mFlushFlagOnShutdown);
		//	msdfgen::deinitializeFreetype(FT_Handle);
		}

		void AssetManager::FlushContainers(ASSET_MANAGER_FLUSH_FLAGS flag)
		{
			/*for (auto& it : mDefaultLibrary.mImportedMaterials) {
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
			mDefaultLibrary.mImportedMeshes.Release();

			mDefaultLibrary.mImportedImages.Release();

			mDefaultLibrary.mImportedMaterials.Release();

			mDefaultLibrary.mImportedAnimations.Release();

			mDefaultLibrary.mImportedAudioClips.Release();
		}

		void AssetManager::Initialize(AssetManagerDesc desc)
		{
			mDesc = desc;
			DefaultBlackTex = Import("Assets/NuclearEngine/DefaultTextures/Black32x32.png", Importers::ImageLoadingDesc());
			DefaultGreyTex = Import("Assets/NuclearEngine/DefaultTextures/Grey32x32.png", Importers::ImageLoadingDesc());
			DefaultWhiteTex = Import("Assets/NuclearEngine/DefaultTextures/White32x32.png", Importers::ImageLoadingDesc());

			DefaultDiffuseTex = DefaultGreyTex;
			DefaultSpecularTex = DefaultWhiteTex;
			DefaultNormalTex = DefaultBlackTex;

			DefaultGreyTex.SetUsageType(Graphics::TextureUsageType::Diffuse);
			DefaultDiffuseTex.SetUsageType(Graphics::TextureUsageType::Diffuse);

			DefaultWhiteTex.SetUsageType(Graphics::TextureUsageType::Diffuse);
			DefaultSpecularTex.SetUsageType(Graphics::TextureUsageType::Specular);

			DefaultNormalTex.SetUsageType(Graphics::TextureUsageType::Normal);

			FT_Handle = msdfgen::initializeFreetype();

			if (FT_Handle == nullptr)
			{
				NUCLEAR_ERROR("[{0}] Failed To Initialize FreeType library");
			}
		}

		Graphics::Texture AssetManager::Import(const Core::Path & Path, const Importers::ImageLoadingDesc& Desc, const Graphics::TextureUsageType& type)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);
			Graphics::Texture result;

			//Check if exists
			auto it = mDefaultLibrary.mImportedImages.mData.find(hashedname);
			if (it != mDefaultLibrary.mImportedImages.mData.end())
			{
				result.SetImage(&it->second);
				return result;
			}


			//Load
			Assets::ImageData imagedata = mImageImporter(Path.mRealPath, Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[{0}] Failed To Load Texture: '{1}' Hash: '{2}'" , mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
				return DefaultBlackTex;
			}

			//Create
			Assets::Image image(imagedata, Desc);
			
			if (image.mTextureView == nullptr)
			{
				NUCLEAR_ERROR("[{0}] Failed To Create Texture: '{1}' Hash: '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

				return DefaultBlackTex;
			}
			image.mData.mData = NULL;

			mDefaultLibrary.mImportedImages.SavePath(hashedname, Path);

			image.SetName(Path.mInputPath);
			mDefaultLibrary.mImportedImages.mData[hashedname] = image;

			result.SetImage(&mDefaultLibrary.mImportedImages.mData[hashedname]);
			result.SetUsageType(type);

			NUCLEAR_INFO("[{0}] Imported Texture: '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
		
			return result;
		}

		Graphics::Texture AssetManager::Import(const Assets::ImageData& imagedata, const Importers::ImageLoadingDesc& Desc)
		{
			assert(Desc.mPath != std::string(""));

			Graphics::Texture result;
			auto hashedname = Utilities::Hash(Desc.mPath);

			//Create
			Assets::Image image(imagedata, Desc);

			if (image.mTextureView == nullptr)
			{
				NUCLEAR_ERROR("[{0}] Failed To Create Texture: '{1}' Hash: '{2}'", mDesc.mName, Desc.mPath, Utilities::int_to_hex<Uint32>(hashedname));
				return DefaultBlackTex;
			}
			image.mData.mData = NULL;
			mDefaultLibrary.mImportedImages.mData[hashedname] = image;
			result.SetImage(&mDefaultLibrary.mImportedImages.mData[hashedname]);

			NUCLEAR_INFO("[{0}] Imported Texture: '{1}' : '{2}'", mDesc.mName, Desc.mPath, Utilities::int_to_hex<Uint32>(hashedname));

			return result;
		}

		//Assets::Image* AssetManager::Import(const Assets::ImageData& Image , const Importers::ImageLoadingDesc & Desc)
		//{
		//	//if (Desc.mPath != std::string(""))
		//	//{
		//	//	auto hashedname = Utilities::Hash(Desc.mPath);

		//	//	
		//	//	Internal::CreateTextureViewFromRawImage(Image, Desc, &mDefaultLibrary.mImportedImages[hashedname]);

		//	//}

		//	if (AddToLibrary)
		//	{

		//	}

		//	Assets::Texture Tex;
		//	Internal::CreateTextureViewFromRawImage(Image, Desc, &Tex);
		//	return Tex;
		//}

		Assets::AudioClip* AssetManager::Import(const Core::Path& Path, AUDIO_IMPORT_MODE mode)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			mDefaultLibrary.mImportedAudioClips.mData[hashedname] = Assets::AudioClip();
			auto result = &mDefaultLibrary.mImportedAudioClips.mData[hashedname];
			Audio::AudioEngine::GetSystem()->createSound(Path.mRealPath.c_str(), mode, 0, &result->mSound);

			NUCLEAR_INFO("[{0}] Imported AudioClip: '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			return result;
		}


		std::tuple<Assets::Mesh*, Assets::Material*, Assets::Animations*> AssetManager::Import(const Core::Path& Path, const Importers::MeshLoadingDesc& desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			//Check if exists
			auto itmesh = mDefaultLibrary.mImportedMeshes.mData.find(hashedname);
			if (itmesh != mDefaultLibrary.mImportedMeshes.mData.end())
			{
				Assets::Animations* anim = nullptr;
				auto itanim = mDefaultLibrary.mImportedAnimations.mData.find(hashedname);
				if (itanim != mDefaultLibrary.mImportedAnimations.mData.end())
				{
					anim = &itanim->second;
				}

				return { &itmesh->second, &mDefaultLibrary.mImportedMaterials.mData[hashedname], anim };
			}

			Assets::Animations Animation;
			Assets::Animations* anim = nullptr;
		
			mDefaultLibrary.mImportedMeshes.mData[hashedname] = Assets::Mesh();
			Assets::Mesh* Mesh = &mDefaultLibrary.mImportedMeshes.mData[hashedname];
			Assets::Material* Material = nullptr;
			if (desc.LoadMaterial)
			{
				mDefaultLibrary.mImportedMaterials.mData[hashedname] = Assets::Material();
				Material = &mDefaultLibrary.mImportedMaterials.mData[hashedname];
			}
			if (!mMeshImporter({ Path.mRealPath.c_str(), desc, this}, Mesh, Material, &Animation))
			{
				NUCLEAR_ERROR("[{0}] Failed to import Model : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

				return { Mesh , Material, anim };
			}

			if (desc.LoadAnimation)
			{
				if (Animation.isValid == true)
				{
					mDefaultLibrary.mImportedAnimations.mData[hashedname] = Animation;
					anim = &mDefaultLibrary.mImportedAnimations.mData[hashedname];

					mDefaultLibrary.mImportedAnimations.SavePath(hashedname, Path);
				}
			}
			NUCLEAR_INFO("[{0}] Imported Model : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			Mesh->Create();
			
			mDefaultLibrary.mImportedMeshes.SavePath(hashedname, Path);

			if (desc.LoadMaterial)
			{
				mDefaultLibrary.mImportedMaterials.SavePath(hashedname, Path);
			}

			return { Mesh , Material, anim };
		}

		template <int N>
		void fillfontdesc(const msdfgen::BitmapConstRef<Byte, N>& bitmap, Assets::FontCreationDesc& desc)
		{
			desc.AtlasWidth = bitmap.width;
			desc.AtlasHeight = bitmap.height;
			desc.Data = bitmap.pixels;
		}
		Assets::Font* AssetManager::Import(const Core::Path& Path, const Importers::FontLoadingDesc& desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);

			using namespace msdf_atlas;
			
			if (msdfgen::FontHandle* font = msdfgen::loadFont(FT_Handle, Path.mRealPath.c_str())) {
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
				
				mDefaultLibrary.mImportedFonts.SavePath(hashedname, Path);
				mDefaultLibrary.mImportedFonts.mData[hashedname] = Assets::Font();
				Assets::Font* result = &mDefaultLibrary.mImportedFonts.mData[hashedname];

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

				NUCLEAR_INFO("[{0}] Imported Font : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

				// Cleanup
				msdfgen::destroyFont(font);
				return result;
			}
			else {
				NUCLEAR_ERROR("[{0}] Failed to import font : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
			}

			return nullptr;
		}

					
		//Graphics::Texture AssetManager::SaveToImport(const Core::Path& Path, const Importers::ImageLoadingDesc& Desc, const Graphics::TextureUsageType& type)
	//	{
			//auto hashedname = Utilities::Hash(Path.mInputPath);
			////Check if exists
			//auto it = mDefaultLibrary.mImportedImages.mData.find(hashedname);
			//if (it != mDefaultLibrary.mImportedImages.mData.end())
			//{
			//	result.SetImage(&it->second);
			//	return result;
			//}




			//Graphics::Texture result;

			////Check if exists
			//auto it = mDefaultLibrary.mImportedImages.mData.find(hashedname);
			//if (it != mDefaultLibrary.mImportedImages.mData.end())
			//{
			//	result.SetImage(&it->second);
			//	return result;
			//}


			////Load
			//Assets::ImageData imagedata = mImageImporter(Path.mRealPath, Desc);
			//if (imagedata.mData == nullptr)
			//{
			//	NUCLEAR_ERROR("[{0}] Failed To Load Texture: '{1}' Hash: '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
			//	return DefaultBlackTex;
			//}

			////Create
			//Assets::Image image(imagedata, Desc);

			//if (image.mTextureView == nullptr)
			//{
			//	NUCLEAR_ERROR("[{0}] Failed To Create Texture: '{1}' Hash: '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			//	return DefaultBlackTex;
			//}
			//image.mData.mData = NULL;

			//mDefaultLibrary.mImportedImages.SavePath(hashedname, Path);

			//image.SetName(Path.mInputPath);
			//mDefaultLibrary.mImportedImages.mData[hashedname] = image;

			//result.SetImage(&mDefaultLibrary.mImportedImages.mData[hashedname]);
			//result.SetUsageType(type);

			//NUCLEAR_INFO("[{0}] Imported Texture: '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			//return result;
		//}

		Assets::Image* AssetManager::TextureCube_Load(const Core::Path& Path, const Importers::ImageLoadingDesc& Desc)
		{
			auto hashedname = Utilities::Hash(Path.mInputPath);
			auto doesitexist = DoesImageExist(hashedname);
			if (doesitexist != nullptr)
			{
				return doesitexist;
			}

			Assets::Image result;
			Assets::ImageData imagedata = mImageImporter(Path.mRealPath, Desc);
			if (imagedata.mData == nullptr)
			{
				NUCLEAR_ERROR("[{0}] Failed To Load Texture2D (For CubeMap): '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));
				return nullptr;
			}
			result.mData = imagedata;

			mDefaultLibrary.mImportedImages.mData[hashedname] = result;
			
			NUCLEAR_INFO("[{0}] Imported Texture2D (for CubeMap) : '{1}' : '{2}'", mDesc.mName, Path.mInputPath, Utilities::int_to_hex<Uint32>(hashedname));

			return &mDefaultLibrary.mImportedImages.mData[hashedname];
		}
		Assets::Image* AssetManager::DoesImageExist(Uint32 hashedname)
		{
			//Check if Texture has been Imported before
			auto it = mDefaultLibrary.mImportedImages.mData.find(hashedname);
			if (it != mDefaultLibrary.mImportedImages.mData.end())
			{
				return &it->second;
			}
			return nullptr;
		}
		std::array<Assets::Image*, 6> AssetManager::LoadTextureCubeFromFile(const std::array<Core::Path, 6>& Paths, const Importers::ImageLoadingDesc& desc)
		{
			Importers::ImageLoadingDesc Desc = desc;
			//Desc.FlipY_Axis = false;

			std::array<Assets::Image*, 6> result = { TextureCube_Load(Paths.at(0), Desc),
				TextureCube_Load(Paths.at(1), Desc), 
				TextureCube_Load(Paths.at(2), Desc), 
				TextureCube_Load(Paths.at(3), Desc),
				TextureCube_Load(Paths.at(4), Desc),
				TextureCube_Load(Paths.at(5), Desc) };

			return result;
		}

		void AssetManager::SaveScene(ECS::Scene* scene)
		{
			std::stringstream storage;

			{
				// output finishes flushing its contents when it goes out of scope
			/*	cereal::JSONOutputArchive output{storage};
				entt::snapshot{ scene->GetRegistry()}.entities(output).component<
					Components::EntityInfoComponent/*,
					Components::CameraComponent,
Components::MeshComponent,
Components::ColliderComponent,
Components::RigidBodyComponent,
Components::DirLightComponent,
Components::PointLightComponent,
Components::SpotLightComponent,
Components::AnimatorComponent,
Components::ScriptComponent*/

		///		>(output);
			}
		}

		//Assets::Script& AssetManager::ImportScript(const Core::Path& Path)
		//{
		//	Assets::Script result;

		//	//result.ScriptCode = Core::FileSystem::LoadFileToString(Path);

		//	return result;
		//}
	}
}