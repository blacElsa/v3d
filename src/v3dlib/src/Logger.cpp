//
// Created by vvin on 6/10/23.
//

#include <v3d/core/Logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace v3d::core {
void Log::Init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");

  m_core_logger = spdlog::stdout_color_mt("v3d");
  m_core_logger->set_level(spdlog::level::trace);

  m_client_logger = spdlog::stdout_color_mt("app");
  m_client_logger->set_level(spdlog::level::trace);
}
}