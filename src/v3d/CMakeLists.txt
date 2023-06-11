add_executable(v3d main.cpp)

target_link_libraries(v3d v3d::v3d_options v3d::v3d_warnings)
target_link_libraries(v3d v3dlib)

set_target_properties(v3d PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED ON
                      CXX_EXTENSIONS OFF)