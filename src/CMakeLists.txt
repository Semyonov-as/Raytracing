add_library(Vec3.hpp INTERFACE)
add_library(Point3.hpp INTERFACE)
add_library(Color.hpp INTERFACE)
add_library(Ray.hpp INTERFACE)

target_compile_features(Vec3.hpp INTERFACE cxx_std_11)
target_compile_features(Point3.hpp INTERFACE cxx_std_11)
target_compile_features(Color.hpp INTERFACE cxx_std_11)
target_compile_features(Ray.hpp INTERFACE cxx_std_11)
