set(sources
  entity.h
  entity.cc
  mesh.h
  mesh.cc
  mesh_factory.h
  mesh_interface.h
  mesh_interface.cc
)

add_library(lf.mesh ${sources})
target_link_libraries(lf.mesh PUBLIC Eigen3::Eigen lf.base lf.geometry lf.mesh.utils)
target_compile_features(lf.mesh PUBLIC cxx_std_17)

add_subdirectory(hybrid2d)
add_subdirectory(utils)
add_subdirectory(test_utils)
