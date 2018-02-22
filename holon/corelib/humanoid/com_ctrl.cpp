/* com_ctrl - COM Controller
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

#include "holon/corelib/humanoid/com_ctrl.hpp"

#include <roki/rk_g.h>
#include <memory>

namespace holon {

void ComCtrlCommands::clear() {
  xd = nullopt;
  yd = nullopt;
  zd = nullopt;
  vxd = nullopt;
  vyd = nullopt;
  qx1 = nullopt;
  qx2 = nullopt;
  qy1 = nullopt;
  qy2 = nullopt;
  qz1 = nullopt;
  qz2 = nullopt;
}

void ComCtrlCommands::set_com_position(const Vec3D& t_com_position) {
  xd = t_com_position.x();
  yd = t_com_position.y();
  zd = t_com_position.z();
}

void ComCtrlCommands::set_com_position(optional<double> t_xd,
                                       optional<double> t_yd,
                                       optional<double> t_zd) {
  xd = t_xd;
  yd = t_yd;
  zd = t_zd;
}

void ComCtrlCommands::set_com_velocity(optional<double> t_vxd,
                                       optional<double> t_vyd) {
  vxd = t_vxd;
  vyd = t_vyd;
}

std::shared_ptr<ComCtrlCommands> ComCtrlCommandsFactory() {
  return std::make_shared<ComCtrlCommands>();
}

std::shared_ptr<ComCtrlInputs> ComCtrlInputsFactory() {
  return std::make_shared<ComCtrlInputs>();
}

std::shared_ptr<ComCtrlOutputs> ComCtrlOutputsFactory() {
  return std::make_shared<ComCtrlOutputs>();
}

ComCtrl::ComCtrl()
    : m_x(),
      m_y(),
      m_model(),
      m_states_ptr(m_model.data_ptr()),
      m_inputs_ptr(ComCtrlInputsFactory()),
      m_outputs_ptr(ComCtrlOutputsFactory()),
      m_user_cmds_ptr(ComCtrlCommandsFactory()),
      m_initial_com_position(m_states_ptr->com_position) {}

ComCtrl& ComCtrl::set_time_step(double t_time_step) {
  m_model.set_time_step(t_time_step);
  return *this;
}

ComCtrl& ComCtrl::reset(const Vec3D& t_com_position) {
  m_initial_com_position = t_com_position;
  m_model.reset(m_initial_com_position);
  return *this;
}

Vec3D ComCtrl::computeDesReactForce(const Vec3D& t_ref_com_position,
                                    const Vec3D& t_com_position,
                                    const Vec3D& t_com_velocity,
                                    double t_mass) const {
  double fz = m_z.computeDesReactForce(t_ref_com_position, t_com_position,
                                       t_com_velocity, t_mass);
  return Vec3D(0, 0, fz);
}

double ComCtrl::computeDesZeta(const Vec3D& t_reaction_force) const {
  return m_model.computeZeta(m_states_ptr->com_position,
                             m_states_ptr->zmp_position, t_reaction_force,
                             m_states_ptr->mass);
}

Vec3D ComCtrl::computeDesZmpPos(const Vec3D& t_ref_com_pos,
                                const Vec3D& t_com_pos, const Vec3D& t_com_vel,
                                double t_desired_zeta) const {
  double xz =
      m_x.computeDesZmpPos(t_ref_com_pos, t_com_pos, t_com_vel, t_desired_zeta);
  double yz =
      m_y.computeDesZmpPos(t_ref_com_pos, t_com_pos, t_com_vel, t_desired_zeta);
  double zz = 0;
  return Vec3D(xz, yz, zz);
}

void ComCtrl::remapUserCommandsToInputs() {
  m_inputs_ptr->com_position[0] =
      cmds().xd.value_or(m_initial_com_position.x());
  m_inputs_ptr->com_position[1] =
      cmds().yd.value_or(m_initial_com_position.y());
  m_inputs_ptr->com_position[2] =
      cmds().zd.value_or(m_initial_com_position.z());
  m_inputs_ptr->com_velocity[0] = cmds().vxd.value_or(0);
  m_inputs_ptr->com_velocity[1] = cmds().vyd.value_or(0);
  m_inputs_ptr->qx1 = cmds().qx1.value_or(ComCtrlX::default_q1);
  m_inputs_ptr->qx2 = cmds().qx2.value_or(ComCtrlX::default_q2);
  m_inputs_ptr->qy1 = cmds().qy1.value_or(ComCtrlY::default_q1);
  m_inputs_ptr->qy2 = cmds().qy2.value_or(ComCtrlY::default_q2);
  m_inputs_ptr->qz1 = cmds().qz1.value_or(ComCtrlZ::default_q1);
  m_inputs_ptr->qz2 = cmds().qz2.value_or(ComCtrlZ::default_q2);
}

void ComCtrl::updateCtrlParam() {
  x().set_q1(inputs().qx1);
  x().set_q2(inputs().qx2);
  y().set_q1(inputs().qy1);
  y().set_q2(inputs().qy2);
  z().set_q1(inputs().qz1);
  z().set_q2(inputs().qz2);
}

bool ComCtrl::update() {
  // remap commanded values given by user to referential values for controller
  remapUserCommandsToInputs();

  // update control parameters
  updateCtrlParam();

  // compute desired reaction force along z-axis
  m_outputs_ptr->reaction_force =
      computeDesReactForce(inputs().com_position, states().com_position,
                           states().com_velocity, states().mass);

  // compute desired value of zeta from desired reaction force
  m_outputs_ptr->zeta = computeDesZeta(m_outputs_ptr->reaction_force);
  if (zIsTiny(outputs().zeta)) return false;

  // compute desired ZMP position
  m_outputs_ptr->zmp_position =
      computeDesZmpPos(inputs().com_position, states().com_position,
                       states().com_velocity, outputs().zeta);

  // update states of COM-ZMP model
  m_states_ptr->reaction_force = outputs().reaction_force;
  m_states_ptr->zmp_position = outputs().zmp_position;
  if (!m_model.update()) return false;

  // update outputs of the controller
  m_outputs_ptr->com_position = states().com_position;
  m_outputs_ptr->com_velocity = states().com_velocity;
  m_outputs_ptr->com_acceleration = states().com_acceleration;
  return true;
}

bool ComCtrl::update(double t_time_step) {
  set_time_step(t_time_step);
  return update();
}

}  // namespace holon
