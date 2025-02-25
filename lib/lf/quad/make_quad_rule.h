/**
 * @file
 * @brief Declaration of various routines for creating QuadRule objects
 * @author Raffael Casagrande
 * @date   2018-08-19 06:29:37
 * @copyright MIT License
 */

#include <lf/base/base.h>
#include "quad_rule.h"

namespace lf::quad {

/**
 * @brief Returns a QuadRule object for the given Reference Element and Order
 * @param ref_el The type of reference element
 * @param order The minimum order that the QuadRule object should have.
 * @return A QuadRule object for the given reference element and with an order
 * >= `order`
 *
 * This method tries to return optimal quadrature rules when possible:
 * - For Segments it returns Gauss-Legendre quadrature rules
 * - For Triangles, it returns specific, hard-coded quadrature rules up to order
 * 50, afterwards, the Duffy-Transform to map a tensor Gaussian Quadrature on a
 * Square to the triangle.
 * - For Quadrilaterals it uses tensor products of Gauss-Legendre rules
 */
QuadRule make_QuadRule(base::RefEl ref_el, unsigned char order);
}  // namespace lf::quad
