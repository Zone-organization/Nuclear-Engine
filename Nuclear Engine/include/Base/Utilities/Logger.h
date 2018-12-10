#pragma once
#include <Base\NE_PrivateCommon.h>
#include <string>
#include <stdarg.h>
#include "ThirdParty\spdlog\spdlog.h"
#include "ThirdParty\spdlog/sinks/stdout_color_sinks.h"

#pragma warning( disable : 4251)

namespace NuclearEngine {
	namespace Base {
		namespace Utilities {

			class NEAPI Logger
			{
			public:
				template<typename... Args>
				inline void Trace(const char *fmt, const Args &... args)
				{
					return mLogger->trace(fmt, args...);
				}

				template<typename... Args>
				inline void Debug(const char *fmt, const Args &... args)
				{
					return mLogger->debug(fmt, args...);
				}

				template<typename... Args>
				inline void Info(const char *fmt, const Args &... args) {
					return mLogger->info(fmt, args...);
				}

				template<typename... Args>
				inline void Warning(const char *fmt, const Args &... args) {
					return mLogger->warn(fmt, args...);
				}

				template<typename... Args>
				inline void Error(const char *fmt, const Args &... args){
					return mLogger->error(fmt, args...);
					}

				template<typename... Args>
				inline void FatalError(const char *fmt, const Args &... args) {
					return mLogger->critical(fmt, args...);
				}

				template<typename... Args>
				inline void Trace(const std::string& fmt, const Args &... args) {
					return mLogger->trace(fmt.c_str(), args...);
				}

				template<typename... Args>
				inline void Debug(const std::string& fmt, const Args &... args) {
					return mLogger->debug(fmt.c_str(), args...);
				}

				template<typename... Args>
				inline void Info(const std::string& fmt, const Args &... args) {
					return mLogger->info(fmt.c_str(), args...);
				}

				template<typename... Args>
				inline void Warning(const std::string& fmt, const Args &... args) {
					return mLogger->warn(fmt.c_str(), args...);
				}

				template<typename... Args>
				inline void Error(const std::string& fmt, const Args &... args) {
					return mLogger->error(fmt.c_str(), args...);
				}

				template<typename... Args>
				inline void FatalError(const std::string& fmt, const Args &... args) {
					return mLogger->critical(fmt.c_str(), args...);
				}

				void Initialize()
				{
					mLogger = spdlog::stdout_color_mt("console");
				}

			private:
				std::shared_ptr<spdlog::logger> mLogger;
			};

		}
	}
		static Base::Utilities::Logger Log;
}