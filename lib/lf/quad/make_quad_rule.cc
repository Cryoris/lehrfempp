/**
 * @file
 * @brief Implementations from make_quad_rule.h
 * @author Raffael Casagrande
 * @date   2018-08-19 06:35:16
 * @copyright MIT License
 */

#include "make_quad_rule.h"
#include <Eigen/KroneckerProduct>
#include "gauss_quadrature.h"

namespace lf::quad {
namespace detail {
template <base::RefElType REF_EL, int Order>
QuadRule HardcodedQuadRule();
}

QuadRule make_QuadRule(base::RefEl ref_el, unsigned char order) {
  if (ref_el == base::RefEl::kSegment()) {
    quadOrder_t n = order / 2 + 1;
    auto [points, weights] = GaussLegendre(n);
    return QuadRule(base::RefEl::kSegment(), points.transpose(),
                    std::move(weights), 2 * n - 1);
  }
  if (ref_el == base::RefEl::kQuad()) {
    quadOrder_t n = order / 2 + 1;
    auto [points1d, weights1d] = GaussLegendre(n);
    Eigen::MatrixXd points2d(2, n * n);
    points2d.row(0) = Eigen::kroneckerProduct(points1d.transpose(),
                                              Eigen::MatrixXd::Ones(1, n));
    points2d.row(1) = points1d.transpose().replicate(1, n);
    return QuadRule(base::RefEl::kQuad(), std::move(points2d),
                    Eigen::kroneckerProduct(weights1d, weights1d), 2 * n - 1);
  }
  if (ref_el == base::RefEl::kTria()) {
    switch (order) {
      case 1:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 1>();
      case 2:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 2>();
      case 3:  // user order 4 rule instead
      case 4:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 4>();
      case 5:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 5>();
      case 6:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 6>();
      case 7:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 7>();
      case 8:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 8>();
      case 9:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 9>();
      case 10:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 10>();
      case 11:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 11>();
      case 12:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 12>();
      case 13:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 13>();
      case 14:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 14>();
      case 15:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 15>();
      case 16:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 16>();
      case 17:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 17>();
      case 18:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 18>();
      case 19:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 19>();
      case 20:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 20>();
      case 21:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 21>();
      case 22:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 22>();
      case 23:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 23>();
      case 24:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 24>();
      case 25:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 25>();
      case 26:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 26>();
      case 27:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 27>();
      case 28:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 28>();
      case 29:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 29>();
      case 30:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 30>();
      case 31:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 31>();
      case 32:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 32>();
      case 33:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 33>();
      case 34:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 34>();
      case 35:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 35>();
      case 36:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 36>();
      case 37:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 37>();
      case 38:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 38>();
      case 39:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 39>();
      case 40:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 40>();
      case 41:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 41>();
      case 42:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 42>();
      case 43:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 43>();
      case 44:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 44>();
      case 45:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 45>();
      case 46:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 46>();
      case 47:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 47>();
      case 48:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 48>();
      case 49:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 49>();
      case 50:
        return detail::HardcodedQuadRule<base::RefEl::kTria(), 50>();
      default:
        // Create a quadrule using tensor product quadrature rule + duffy
        // transform
        quadOrder_t n = order / 2 + 1;
        auto [leg_p, leg_w] = GaussLegendre(n);
        auto [jac_p, jac_w] = GaussJacobi(n, 1, 0);
        jac_p.array() = (jac_p.array() + 1) / 2.;  // rescale to [0,1]
        jac_w.array() *= 0.25;
        Eigen::MatrixXd points2d(2, n * n);
        points2d.row(0) = Eigen::kroneckerProduct(
            leg_p.transpose(), (1 - jac_p.transpose().array()).matrix());
        points2d.row(1) = jac_p.transpose().replicate(1, n);
        return QuadRule(base::RefEl::kTria(), std::move(points2d),
                        Eigen::kroneckerProduct(leg_w, jac_w), 2 * n - 1);
    }
  }
  LF_VERIFY_MSG(
      false, "No Quadrature rules implemented for this reference element yet.");
}
}  // namespace lf::quad
