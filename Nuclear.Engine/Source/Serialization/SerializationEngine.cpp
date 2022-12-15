#include "Serialization/SerializationEngine.h"
#include "Assets/Scene.h"
#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/string.h>
#include <Serialization/ComponentsSerialization.h>
#include <Components\Components.h>


namespace Nuclear
{
	namespace Serialization
	{
		
		// some helper types
		using Buffer = std::vector<uint8_t>;
		using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
		using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

		bool SerializationEngine::SaveScene(Assets::Scene* scene)
		{
			//data stored here
			//Buffer buffer;
			//entt::snapshot{ scene->GetRegistry() }.entities(buffer).component<Components::EntityInfoComponent>(buffer);		
		//	auto buffersize = bitsery::quickSerialization<OutputAdapter>(buffer, data);


			Components::EntityInfoComponent testcomp;
			testcomp.mName = "hahah645645ahahaha";
			testcomp.mTransform.SetPosition({ 69.f, 565.f, 676.f });


			// create buffer to store data
			Buffer buffer;
			// use quick serialization function,
			// it will use default configuration to setup all the nesessary steps
			// and serialize data to container
			auto writtenSize = bitsery::quickSerialization<OutputAdapter>(buffer, testcomp);

			Components::EntityInfoComponent testcomp2;

			auto state = bitsery::quickDeserialization<InputAdapter>(
				{ buffer.begin(), writtenSize }, testcomp2);


			//std::stringstream storage;

			//{
			//	// output finishes flushing its contents when it goes out of scope
			//	cereal::JSONOutputArchive output{ storage };
			//	entt::snapshot{ GetRegistry() }.entities(output).component<
			//		Components::EntityInfoComponent,
			//		Components::MeshComponent

			//		//	Components::DirLightComponent,
			//		//	Components::PointLightComponent,
			//		//	Components::SpotLightComponent
			//	>(output);
			//}

			//cereal::JSONInputArchive input{ storage };
			//entt::snapshot_loader{ destination }.entities(input).component<Components::EntityInfoComponent, Components::MeshComponent
			//>(input);

			return true;
		}
	}
}