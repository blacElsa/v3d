add_library(v3dlib STATIC src/v3dlib.cpp src/GLFWwindow.cpp src/Logger.cpp src/Event.cpp)

add_library(v3d::v3dlib ALIAS v3dlib)

find_package(OpenGL REQUIRED)

target_link_libraries(v3dlib
                      PRIVATE v3d_options v3d_warnings
                      PUBLIC glfw OpenGL::GL spdlog glad)

target_include_directories(v3dlib ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                          $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
set_target_properties(v3dlib PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED ON
                      CXX_EXTENSIONS OFF)