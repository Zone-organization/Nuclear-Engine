#pragma once
#include <NE_Compiler.h>
#include <spdlog/logger.h>

#pragma warning( disable : 4251)

namespace Nuclear
{
	namespace Utilities
	{
		class NEAPI Logger
		{
		public:
			Logger(Logger const&) = delete;
			void operator=(Logger const&) = delete;

			FORCE_INLINE std::shared_ptr<spdlog::logger>& GetEngineLogger() { return mEngineLogger; }
			FORCE_INLINE std::shared_ptr<spdlog::logger>& GetClientLogger() { return mClientLogger; }

			FORCE_INLINE static Logger& Get()
			{
				static Logger logger;

				return logger;
			}

		protected:
			std::shared_ptr<spdlog::logger> mEngineLogger;
			std::shared_ptr<spdlog::logger> mClientLogger;
		private:
			Logger();
		};
	}
}

// engine logger
#define NUCLEAR_TRACE(...)    Nuclear::Utilities::Logger::Get().GetEngineLogger()->trace(__VA_ARGS__)
#define NUCLEAR_INFO(...)     Nuclear::Utilities::Logger::Get().GetEngineLogger()->info(__VA_ARGS__)
#define NUCLEAR_WARN(...)     Nuclear::Utilities::Logger::Get().GetEngineLogger()->warn(__VA_ARGS__)
#define NUCLEAR_ERROR(...)    Nuclear::Utilities::Logger::Get().GetEngineLogger()->error(__VA_ARGS__)
#define NUCLEAR_FATAL(...) Nuclear::Utilities::Logger::Get().GetEngineLogger()->critical(__VA_ARGS__)

// Client logger
#define CLIENT_TRACE(...)         Nuclear::Utilities::Logger::Get().GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_INFO(...)          Nuclear::Utilities::Logger::Get().GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_WARN(...)          Nuclear::Utilities::Logger::Get().GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)         Nuclear::Utilities::Logger::Get().GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_FATAL(...)      Nuclear::Utilities::Logger::Get().GetClientLogger()->critical(__VA_ARGS__)