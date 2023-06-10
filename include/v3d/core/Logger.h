//
// Created by vvin on 6/10/23.
//

#ifndef V3D_LOGGER_H
#define V3D_LOGGER_H

#include <memory>

#include <spdlog/spdlog.h>

namespace v3d::core {
class Log {
public:
  static void Init();

  static std::shared_ptr<spdlog::logger> GetCoreLogger() { return m_core_logger; }
  static std::shared_ptr<spdlog::logger> GetClientLogger() { return m_client_logger; }
private:
  inline static std::shared_ptr<spdlog::logger> m_core_logger;
  inline static std::shared_ptr<spdlog::logger> m_client_logger;
};
}

#define V3D_CORE_TRACE(...)       ::v3d::core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define V3D_CORE_INFO(...)        ::v3d::core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define V3D_CORE_WARN(...)        ::v3d::core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define V3D_CORE_ERROR(...)       ::v3d::core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define V3D_CORE_CRITICAL(...)    ::v3d::core::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define APP_CORE_TRACE(...)       ::v3d::core::Log::GetClientLogger()->trace(__VA_ARGS__)
#define APP_CORE_INFO(...)        ::v3d::core::Log::GetClientLogger()->info(__VA_ARGS__)
#define APP_CORE_WARN(...)        ::v3d::core::Log::GetClientLogger()->warn(__VA_ARGS__)
#define APP_CORE_ERROR(...)       ::v3d::core::Log::GetClientLogger()->error(__VA_ARGS__)
#define APP_CORE_CRITICAL(...)    ::v3d::core::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif //V3D_LOGGER_H
