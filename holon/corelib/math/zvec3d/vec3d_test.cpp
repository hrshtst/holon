/* vec3d - 3D vector that wraps zVec3D
 *
 * Copyright (c) 2018 Hiroshi Atsuta <atsuta.hiroshi@gmail.com>
 *
 * This file is part of the holon.
 *
 * The holon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The holon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the holon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "holon/corelib/math/zvec3d/vec3d.hpp"

#include "catch.hpp"
#include "holon/test/util/catch/custom_matchers.hpp"
#include "holon/test/util/fuzzer/fuzzer.hpp"

namespace holon {
namespace math {
namespace zvec3d {
namespace {

using Catch::Matchers::Equals;

TEST_CASE("zvec3d::Vec3D: constructors", "[corelib][math][Vec3D]") {
  Fuzzer fuzz;

  SECTION("default constructor should initialize with zeros") {
    Vec3D v;
    CHECK(v.x() == 0.0);
    CHECK(v.y() == 0.0);
    CHECK(v.z() == 0.0);
  }

  SECTION("constructor with three arguments") {
    double x = fuzz.get();
    double y = fuzz.get();
    double z = fuzz.get();
    Vec3D v(x, y, z);
    CHECK(v.x() == x);
    CHECK(v.y() == y);
    CHECK(v.z() == z);
  }
}

TEST_CASE("zvec3d::Vec3D: copy constructor") {
  Vec3D a(1.0, 2.0, 3.0);
  Vec3D b(a);
}

}  // namespace
}  // namespace zvec3d
}  // namespace math
}  // namespace holon
