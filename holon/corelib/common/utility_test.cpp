/* utility - Utility header
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

#include "holon/corelib/common/utility.hpp"

#include "catch.hpp"

namespace holon {
namespace {

TEST_CASE("Check all_are_base_of", "[utility][all_are_base_of]") {
  struct Base {};
  struct Derived : Base {};
  struct NotDerived {};

  CHECK(all_are_base_of<Base, Derived, Derived, Derived>::value);
  CHECK_FALSE(all_are_base_of<Base, Derived, NotDerived>::value);
}

}  // namespace
}  // namespace holon