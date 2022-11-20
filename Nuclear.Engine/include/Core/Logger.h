#pragma once
#include <Core\NE_Common.h>
#include <spdlog/spdlog.h>

#pragma warning( disable : 4251)

namespace Nuclear
{
	namespace Core
	{
		class Logger
		{
		public:
			static void Initialize();
			static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return mEngineLogger; }
			static std::shared_ptr<spdlog::logger>& GetClientLogger() { return mClientLogger; }
		private:

			static std::shared_ptr<spdlog::logger> mEngineLogger;
			static std::shared_ptr<spdlog::logger> mClientLogger;
		};
	}
}

// engine logger
#define NUCLEAR_TRACE(...)    Nuclear::Core::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define NUCLEAR_INFO(...)     Nuclear::Core::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define NUCLEAR_WARN(...)     Nuclear::Core::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define NUCLEAR_ERROR(...)    Nuclear::Core::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define NUCLEAR_FATAL(...) Nuclear::Core::Logger::GetEngineLogger()->critical(__VA_ARGS__)

// Client logger
#define CLIENT_TRACE(...)         Nuclear::Core::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_INFO(...)          Nuclear::Core::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_WARN(...)          Nuclear::Core::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)         Nuclear::Core::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_FATAL(...)      Nuclear::Core::Logger::GetClientLogger()->critical(__VA_ARGS__)