#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace EventBuilder {

	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};

	#define EVB_CRITICAL(...) ::EventBuilder::Logger::GetLogger()->critical(__VA_ARGS__)
	#define EVB_ERROR(...) ::EventBuilder::Logger::GetLogger()->error(__VA_ARGS__)
	#define EVB_WARN(...) ::EventBuilder::Logger::GetLogger()->warn(__VA_ARGS__)
	#define EVB_INFO(...) ::EventBuilder::Logger::GetLogger()->info(__VA_ARGS__)
	#define EVB_TRACE(...) ::EventBuilder::Logger::GetLogger()->trace(__VA_ARGS__)
}

#endif