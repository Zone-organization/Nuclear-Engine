#pragma once
namespace NuclearEngine 
{
	namespace API 
	{
		enum class DataType { Float, Float2, Float3, Float4 };

		enum class BufferUsage {
			Default,
			Static,
			Dynamic
		};

		struct VertexBufferDesc {
			const void* data;
			unsigned int size;
			BufferUsage usage = BufferUsage::Default;
		};
	}
}