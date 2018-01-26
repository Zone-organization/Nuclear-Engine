#pragma once

namespace NuclearEngine {

	enum class DataType { Float, Float2, Float3, Float4 };

	#define	ClearColorBuffer 1
	#define	ClearDepthBuffer 2
	#define	ClearStencilBuffer 4

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