add_library(Vec3 STATIC Vec3.hpp)
add_library(Point3 STATIC Point3.hpp)
add_library(Color STATIC Color.hpp)
add_library(Ray STATIC Ray.hpp)

target_sources(Vec3 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Vec3.hpp)
target_sources(Point3 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Point3.hpp)
target_sources(Color PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Color.hpp)
target_sources(Ray PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Ray.hpp)

target_include_directories(Vec3 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Point3 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Color PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Ray PUBLIC ${CMAKE_CURRENT_LIST_DIR})
