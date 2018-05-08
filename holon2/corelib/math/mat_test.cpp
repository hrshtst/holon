/* mat - Matrix class
 *
 * Copyright (c) 2018 Hiroshi Atsuta <atsuta.hiroshi@gmail.com>
 *
 * This file is part of holon.
 *
 * Holon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Holon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with holon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "holon2/corelib/math/mat.hpp"

#include "third_party/catch/catch.hpp"

namespace holon {
namespace {

TEST_CASE("Example 1 in Eigen tutorial", "[Mat]") {
  Mat m(2, 2);
  m(0, 0) = 3;
  m(1, 0) = 2.5;
  m(0, 1) = -1;
  m(1, 1) = m(1, 0) + m(0, 1);
  CHECK(m(0, 0) == 3);
  CHECK(m(0, 1) == -1);
  CHECK(m(1, 0) == 2.5);
  CHECK(m(1, 1) == Approx(1.5));
}

TEST_CASE("Example 2 in Eigen tutorial: size set at run time", "[Mat][Vec]") {
  Mat m = Mat::Random(3, 3);
  m = (m + Mat::Constant(3, 3, 1.2)) * 50;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      CHECK(m(i, j) > 10);
      CHECK(m(i, j) < 110);
    }
  }

  Vec v(3);
  v << 1, 2, 3;
  CHECK(v(0) == 1);
  CHECK(v(1) == 2);
  CHECK(v(2) == 3);

  Vec mv = m * v;
  CHECK(mv(0) == Approx(m(0, 0) * v(0) + m(0, 1) * v(1) + m(0, 2) * v(2)));
  CHECK(mv(1) == Approx(m(1, 0) * v(0) + m(1, 1) * v(1) + m(1, 2) * v(2)));
  CHECK(mv(2) == Approx(m(2, 0) * v(0) + m(2, 1) * v(1) + m(2, 2) * v(2)));
}

TEST_CASE("Example 2 in Eigen tutorial: size set at compile time",
          "[Mat3d][Vec3d]") {
  Mat3d m = Mat3d::Random();
  m = (m + Mat3d::Constant(1.2)) * 50;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      CHECK(m(i, j) > 10);
      CHECK(m(i, j) < 110);
    }
  }

  Vec3d v(1, 2, 3);
  CHECK(v(0) == 1);
  CHECK(v(1) == 2);
  CHECK(v(2) == 3);

  Vec3d mv = m * v;
  CHECK(mv(0) == Approx(m(0, 0) * v(0) + m(0, 1) * v(1) + m(0, 2) * v(2)));
  CHECK(mv(1) == Approx(m(1, 0) * v(0) + m(1, 1) * v(1) + m(1, 2) * v(2)));
  CHECK(mv(2) == Approx(m(2, 0) * v(0) + m(2, 1) * v(1) + m(2, 2) * v(2)));
}

TEST_CASE("Constant Vectors", "[Vec3d]") {
  CHECK(kVec3dZero == Vec3d(0, 0, 0));
  CHECK(kVec3dX == Vec3d(1, 0, 0));
  CHECK(kVec3dY == Vec3d(0, 1, 0));
  CHECK(kVec3dZ == Vec3d(0, 0, 1));
}

TEST_CASE("allclose for Mat class", "[Mat]") {
  Mat3d m1, m2;
  m1 << 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9;
  m2 << 0.1, 0.2, 0.1 + 0.2, 0.4, 0.5, 0.3 + 0.3, 0.7, 0.8, 0.9;
  CHECK(allclose(m1, m2));
}

}  // namespace
}  // namespace holon
