# Here we want to compile all snippets to make sure that they stay up-to-date, even when the code is refactored.

set(sources
  forward_iterator.cc
  gmsh_reader.cc
  lambda_mesh_data_set.cc
  ref_el.cc
  vtk_writer.cc
  mesh_utils.cc
)

add_library(docsnippets ${sources})
get_target_property(OUT lf.base INCLUDE_DIRECTORIES)
target_link_libraries(docsnippets PUBLIC Eigen3::Eigen lf.base lf.mesh)
target_compile_features(docsnippets PUBLIC cxx_std_17)
