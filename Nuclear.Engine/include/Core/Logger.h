#pragma once
#include <Core\NE_Common.h>
#include <spdlog/spdlog.h>

#pragma warning( disable : 4251)

namespace Nuclear
{
	namespace Core
	{
		class NEAPI Logger
		{
		public:
			Logger(Logger const&) = delete;
			void operator=(Logger const&) = delete;

			std::shared_ptr<spdlog::logger>& GetEngineLogger() { return mEngineLogger; }
			std::shared_ptr<spdlog::logger>& GetClientLogger() { return mClientLogger; }

			static Logger& GetInstance();
		private:
			Logger();
			std::shared_ptr<spdlog::logger> mEngineLogger;
			std::shared_ptr<spdlog::logger> mClientLogger;
		};
	}
}

// engine logger
#define NUCLEAR_TRACE(...)    Nuclear::Core::Logger::GetInstance().GetEngineLogger()->trace(__VA_ARGS__)
#define NUCLEAR_INFO(...)     Nuclear::Core::Logger::GetInstance().GetEngineLogger()->info(__VA_ARGS__)
#define NUCLEAR_WARN(...)     Nuclear::Core::Logger::GetInstance().GetEngineLogger()->warn(__VA_ARGS__)
#define NUCLEAR_ERROR(...)    Nuclear::Core::Logger::GetInstance().GetEngineLogger()->error(__VA_ARGS__)
#define NUCLEAR_FATAL(...) Nuclear::Core::Logger::GetInstance().GetEngineLogger()->critical(__VA_ARGS__)

// Client logger
#define CLIENT_TRACE(...)         Nuclear::Core::Logger::GetInstance().GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_INFO(...)          Nuclear::Core::Logger::GetInstance().GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_WARN(...)          Nuclear::Core::Logger::GetInstance().GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)         Nuclear::Core::Logger::GetInstance().GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_FATAL(...)      Nuclear::Core::Logger::GetInstance().GetClientLogger()->critical(__VA_ARGS__)