#include "EditorUILayer.h"

#include <sstream>

namespace Nuclear::Editor
{

	enum class LogLevel
	{
		None,
		Trace,
		Info,
		Warn,
		Error,
		Fatal,
		All,
	};

	class LoggerView : public EditorUILayer
	{
	public:
		virtual ~LoggerView();

		virtual void Init() override;
		virtual void Update() override;

	private:

	};

}
