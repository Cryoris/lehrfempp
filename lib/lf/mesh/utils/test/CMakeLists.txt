
include(GoogleTest)

set(sources
  count_test.cc
  torus_mesh_builder_tests.cc
  tp_quad_mesh_builder_tests.cc
  tp_triag_mesh_builder_tests.cc
)

add_executable(lf.mesh.utils.test ${sources})
target_link_libraries(lf.mesh.utils.test
  PUBLIC Eigen3::Eigen Boost::boost GTest::main
  lf.mesh.hybrid2d
  lf.mesh.utils
  lf.mesh.test_utils
)
target_compile_features(lf.mesh.utils.test PUBLIC cxx_std_17)
gtest_discover_tests(lf.mesh.utils.test)
