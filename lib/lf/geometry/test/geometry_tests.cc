/**
 * @file
 * @brief tests for geometry objects
 * @author Anian Ruoss
 * @date   2018-10-27 15:57:17
 * @copyright MIT License
 */

#include <gtest/gtest.h>
#include <lf/geometry/geometry.h>
#include <lf/quad/quad.h>

/**
 * Checks if Jacobian() is implemented correctly by comparing it with the
 * symmetric difference quotient approximation
 */
void checkJacobians(const lf::geometry::Geometry &geom,
                    const Eigen::MatrixXd &eval_points,
                    const double tolerance) {
  const double h = 1e-6;

  const size_t num_points = eval_points.cols();
  const size_t dim_local = geom.DimLocal();
  const size_t dim_global = geom.DimGlobal();

  Eigen::MatrixXd jacobians = geom.Jacobian(eval_points);

  EXPECT_EQ(jacobians.rows(), dim_global) << "Jacobian has " << jacobians.rows()
                                          << " rows instead of " << dim_global;
  EXPECT_EQ(jacobians.cols(), num_points * dim_local)
      << "Jacobian has " << jacobians.cols() << " cols instead of "
      << num_points * dim_local;

  for (size_t j = 0; j < num_points; ++j) {
    auto point = eval_points.col(j);

    Eigen::MatrixXd jacobian =
        jacobians.block(0, j * dim_local, dim_global, dim_local);
    Eigen::MatrixXd approx_jacobian =
        Eigen::MatrixXd::Zero(dim_global, dim_local);

    for (size_t i = 0; i < dim_local; ++i) {
      Eigen::VectorXd h_vec = Eigen::VectorXd::Zero(dim_local);
      h_vec(i) = h;

      // approximate gradient with symmetric difference quotient
      approx_jacobian.col(i) =
          (geom.Global(point + h_vec) - geom.Global(point - h_vec)) / (2. * h);
    }

    EXPECT_TRUE(jacobian.isApprox(approx_jacobian, tolerance))
        << "Jacobian incorrect at point " << point;
  }
}

/**
 * Checks if JacobianInverseGramian() is implemented correctly under the
 * assumption that Jacobian() is correct
 */
void checkJacobianInverseGramian(const lf::geometry::Geometry &geom,
                                 const Eigen::MatrixXd &eval_points) {
  const size_t num_points = eval_points.cols();
  const size_t dim_local = geom.DimLocal();
  const size_t dim_global = geom.DimGlobal();

  Eigen::MatrixXd jacobians = geom.Jacobian(eval_points);
  Eigen::MatrixXd jacInvGrams = geom.JacobianInverseGramian(eval_points);

  EXPECT_EQ(jacInvGrams.rows(), dim_global)
      << "JacobianInverseGramian has " << jacInvGrams.rows()
      << " rows instead of " << dim_global;
  EXPECT_EQ(jacInvGrams.cols(), num_points * dim_local)
      << "JacobianInverseGramian has " << jacInvGrams.cols()
      << " cols instead of " << num_points * dim_local;

  for (int j = 0; j < num_points; ++j) {
    Eigen::MatrixXd jacInvGram =
        jacInvGrams.block(0, j * dim_local, dim_global, dim_local);
    Eigen::MatrixXd jacobian =
        jacobians.block(0, j * dim_local, dim_global, dim_local);

    EXPECT_TRUE(jacInvGram.isApprox(
        jacobian * (jacobian.transpose() * jacobian).inverse()))
        << "JacobianInverseGramian incorrect at point " << eval_points.col(j);
  }
}

/**
 * Checks if IntegrationElement() is implemented correctly under the assumption
 * that Jacobian() is correct
 */
void checkIntegrationElement(const lf::geometry::Geometry &geom,
                             const Eigen::MatrixXd &eval_points) {
  const size_t num_points = eval_points.cols();
  const size_t dim_local = geom.DimLocal();
  const size_t dim_global = geom.DimGlobal();

  Eigen::MatrixXd jacobians = geom.Jacobian(eval_points);
  Eigen::VectorXd integrationElements = geom.IntegrationElement(eval_points);

  EXPECT_EQ(integrationElements.rows(), num_points)
      << "IntegrationElement has " << integrationElements.rows()
      << " rows instead of " << num_points;
  EXPECT_EQ(integrationElements.cols(), 1)
      << "IntegrationElement has " << integrationElements.cols()
      << " cols instead of " << 1;

  for (int j = 0; j < num_points; ++j) {
    Eigen::MatrixXd jacobian =
        jacobians.block(0, j * dim_local, dim_global, dim_local);

    const double integrationElement = integrationElements(j);
    const double approx_integrationElement =
        std::sqrt((jacobian.transpose() * jacobian).determinant());

    EXPECT_DOUBLE_EQ(integrationElement, approx_integrationElement)
        << "IntegrationElement incorrect at point " << eval_points.col(j);
  }
}

TEST(Geometry, Point) {
  lf::geometry::Point geom((Eigen::MatrixXd(2, 1) << 1, 1).finished());

  // testing with the Jacobian is of limited value as it is a (2,0) matrix
  Eigen::MatrixXd points = Eigen::MatrixXd::Random(0, 3);
  checkJacobians(geom, points, 1e-9);
  checkJacobianInverseGramian(geom, points);
  // TODO: fix inconsistent dimensions for IntegrationElement()
  // checkIntegrationElement(geom, points);
}

TEST(Geometry, SegmentO1) {
  lf::geometry::SegmentO1 geom(
      (Eigen::MatrixXd(2, 2) << 1, 1, 0, 4).finished());
  auto qr = lf::quad::make_QuadRule(lf::base::RefEl::kSegment(), 5);

  checkJacobians(geom, qr.Points(), 1e-9);
  checkJacobianInverseGramian(geom, qr.Points());
  checkIntegrationElement(geom, qr.Points());
}

TEST(Geometry, TriaO1) {
  lf::geometry::TriaO1 geom(
      (Eigen::MatrixXd(2, 3) << 1, 4, 3, 1, 2, 5).finished());
  auto qr = lf::quad::make_QuadRule(lf::base::RefEl::kTria(), 5);

  checkJacobians(geom, qr.Points(), 1e-9);
  checkJacobianInverseGramian(geom, qr.Points());
  checkIntegrationElement(geom, qr.Points());
}

TEST(Geometry, QuadO1) {
  lf::geometry::QuadO1 geom(
      (Eigen::MatrixXd(2, 4) << -1, 3, 2, 1, -2, 0, 2, 1).finished());
  auto qr = lf::quad::make_QuadRule(lf::base::RefEl::kQuad(), 5);

  checkJacobians(geom, qr.Points(), 1e-9);
  checkJacobianInverseGramian(geom, qr.Points());
  checkIntegrationElement(geom, qr.Points());
}
