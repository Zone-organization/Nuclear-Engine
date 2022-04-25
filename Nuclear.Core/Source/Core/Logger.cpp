#include <Core\Logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Nuclear
{
	namespace Core 
	{
		std::shared_ptr<spdlog::logger> Logger::mEngineLogger;
		std::shared_ptr<spdlog::logger> Logger::mClientLogger;
		void Logger::Initialize()
		{
			std::vector<spdlog::sink_ptr> logSinks;
			logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("NuclearEngine.log", true));

			logSinks[0]->set_pattern("%^[%T] %n: %v%$");
			logSinks[1]->set_pattern("[%T] [%l] %n: %v");

			mEngineLogger = std::make_shared<spdlog::logger>("NUCLEAR", begin(logSinks), end(logSinks));
			spdlog::register_logger(mEngineLogger);
			mEngineLogger->set_level(spdlog::level::trace);
			mEngineLogger->flush_on(spdlog::level::trace);

			mClientLogger = std::make_shared<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));
			spdlog::register_logger(mClientLogger);
			mClientLogger->set_level(spdlog::level::trace);
			mClientLogger->flush_on(spdlog::level::trace);
		}
	}
}