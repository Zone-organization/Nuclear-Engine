#include "NSLParser.h"


namespace Xsc
{
	NSLParser::NSLParser(Log * log)
		: HLSLParser{ log }
	{
	}
	ProgramPtr NSLParser::ParseSource(const SourceCodePtr & source, const NameMangling & nameMangling, const InputShaderVersion versionIn, bool rowMajorAlignment, bool enableWarnings)
	{
		return ProgramPtr();
	}
}