/* com_ctrl_y - COM controller along y axis
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

#include "holon/corelib/humanoid/com_ctrl_y.hpp"

namespace holon {

ComCtrlY::ComCtrlY() : m_q1(default_q1), m_q2(default_q2) {}
ComCtrlY::ComCtrlY(double t_q1, double t_q2) : m_q1(t_q1), m_q2(t_q2) {}

ComCtrlY::~ComCtrlY() = default;

ComCtrlY& ComCtrlY::set_q1(double t_q1) {
  m_q1 = t_q1;
  return *this;
}

ComCtrlY& ComCtrlY::set_q2(double t_q2) {
  m_q2 = t_q2;
  return *this;
}

}  // namespace holon
