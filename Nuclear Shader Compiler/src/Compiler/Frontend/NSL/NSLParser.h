#pragma once

#include "SLParser.h"
#include "HLSLParser.h"
#include "SymbolTable.h"
#include <map>


namespace Xsc
{


	// Syntax parser class for the shading language HLSL.
	class NSLParser : public HLSLParser
	{

	public:

		NSLParser(Log* log = nullptr);

		ProgramPtr ParseSource(
			const SourceCodePtr& source,
			const NameMangling& nameMangling,
			const InputShaderVersion versionIn,
			bool rowMajorAlignment = false,
			bool enableWarnings = false
		);
	};


} // /namespace Xsc


  // ================================================================================
