/* com_controller - COM controller class
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

#include "holon2/corelib/humanoid/com_controller.hpp"
#include "holon2/corelib/humanoid/com_zmp_model/com_zmp_model_formula.hpp"

namespace holon {

namespace cz = com_zmp_model_formula;

const Array3d ComController::default_q1 = {{1, 1, 1}};
const Array3d ComController::default_q2 = {{1, 1, 1}};
const double ComController::default_rho = 0;
const double ComController::default_dist = 0;
const double ComController::default_kr = 1;

ComController::ComController() : ComController(Data()) {}

ComController::ComController(Data t_data)
    : m_data(t_data),
      m_sim(m_data.subdata<0, 1>()),
      m_desired_cf(m_data),
      m_desired_zmp(m_data) {
  ComZmpModelBuilder().build(m_data.subdata<0, 1>());
  setDefaultParameters();
  setupSimulator();
}

ComController::ComController(const Model& t_model) : ComController() {
  copyModelData(t_model);
  reset(states().com_position);
}

void ComController::setDefaultParameters() {
  params().com_position = states().com_position;
  params().com_velocity = kVec3dZero;
  params().q1 = default_q1;
  params().q2 = default_q2;
  params().rho = default_rho;
  params().dist = default_dist;
  params().kr = default_kr;
}

void ComController::setupSimulator() {
  m_sim.setInitialComPosition();
  m_sim.setZmpPosAsInput();
  m_sim.setContactForce(getContactForceFunctor());
  m_sim.setZmpPosition(getZmpPositionFunctor());
}

void ComController::copyModelData(const Model& t_model) {
  m_data.copy(t_model.data(), IndexSeq<0, 1>(), IndexSeq<0, 1>());
}

ComController& ComController::setTimeStep(double t_time_step) {
  m_sim.setTimeStep(t_time_step);
  return *this;
}

ComController& ComController::reset() { return reset(getInitialComPosition()); }

ComController& ComController::reset(const Vec3d& t_com_position) {
  m_sim.reset(t_com_position);
  params().com_position = t_com_position;
  return *this;
}

ComController& ComController::feedback(const Model& t_model) {
  return feedback(t_model.data());
}

ComController& ComController::feedback(const ComZmpModelData& t_model_data) {
  states().com_position = t_model_data.get<1>().com_position;
  states().com_velocity = t_model_data.get<1>().com_velocity;
  return *this;
}

void ComController::updateOutputs() {
  outputs().com_position = states().com_position;
  outputs().com_velocity = states().com_velocity;
  outputs().com_acceleration = states().com_acceleration;
  outputs().zmp_position = states().zmp_position;
  outputs().contact_force = states().contact_force;
}

bool ComController::update() { return update(time_step()); }

bool ComController::update(double t_time_step) {
  m_sim.update(t_time_step);
  updateOutputs();
  return true;
}

ComController::Functor ComController::getContactForceFunctor() const {
  return [this](const Vec3d& p, const Vec3d& v, const double) -> Vec3d {
    return m_desired_cf.calculate(p, v);
  };
}

ComController::Functor ComController::getZmpPositionFunctor() const {
  return [this](const Vec3d& p, const Vec3d& v, const double) -> Vec3d {
    auto fz = m_desired_cf.calculateZ(p.z(), v.z());
    auto zeta = cz::zeta(p.z(), vhp(), fz, mass());
    return m_desired_zmp.calculate(p, v, zeta);
  };
}

}  // namespace holon