/* com_ctrl_x - COM controller along x axis
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

#ifndef HOLON_HUMANOID_COM_CTRL_X_HPP_
#define HOLON_HUMANOID_COM_CTRL_X_HPP_

namespace holon {

class ComCtrlX {
  const double default_q1 = 1.0;
  const double default_q2 = 1.0;

 public:
  ComCtrlX();
  ComCtrlX(double t_q1, double t_q2);
  virtual ~ComCtrlX();

  inline double q1() const noexcept { return m_q1; }
  inline double q2() const noexcept { return m_q2; }

  ComCtrlX& set_q1(double t_q1);
  ComCtrlX& set_q2(double t_q2);

 private:
  double m_q1;
  double m_q2;
};

}  // namespace holon

#endif  // HOLON_HUMANOID_COM_CTRL_X_HPP_
