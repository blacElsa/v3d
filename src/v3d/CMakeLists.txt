add_executable(v3d main.cpp)

target_link_libraries(v3d v3d::v3d_options v3d::v3d_warnings)
target_link_libraries(v3d v3dlib)