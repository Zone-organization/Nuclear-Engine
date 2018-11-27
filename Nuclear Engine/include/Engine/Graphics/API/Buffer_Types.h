#pragma once
namespace NuclearEngine
{
	namespace Graphics
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
				const void* Data;
				unsigned int Size;
				BufferUsage UsageType = BufferUsage::Default;
			};

			struct IndexBufferDesc {
				const void* Data;
				unsigned int Size;
				BufferUsage UsageType = BufferUsage::Default;
				bool UsePreciseSize = false;
			};
		}
	}
}