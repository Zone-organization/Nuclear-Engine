#include "Common.h"
#include "Sample1.h"
#include <iostream>


int main(int argc, char* argv[])
{
	Core::EngineStartupDesc desc;

	desc.mAppdesc.AutoSelectRenderer = false;
	Core::Engine::Start(desc);
	//Sample1 demo;
	//Core::Engine::RunGame(&demo);

	 // Specify vertex format
	LLGL::VertexFormat vertexFormat;
	vertexFormat.AppendAttribute({ "position", LLGL::Format::RG32Float });
	vertexFormat.AppendAttribute({ "texCoord", LLGL::Format::RG32Float });

	// Define vertex buffer data
	struct Vertex
	{
		Math::Vector2 position;
		Math::Vector2 texCoord;
	};

	std::vector<Vertex> vertices =
	{
		{ { -1,  1 }, { -2, -2 } },
		{ { -1, -1 }, { -2,  2 } },
		{ {  1,  1 }, {  2, -2 } },
		{ {  1, -1 }, {  2,  2 } },
	};


	Core::Engine::Shutdown();

	return 0;
}