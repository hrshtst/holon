/* pd_ctrl - Simple PD control class
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

#ifndef HOLON_CONTROL_PD_CTRL_HPP_
#define HOLON_CONTROL_PD_CTRL_HPP_

#include <array>
#include <functional>
#include "holon/corelib/control/ctrl_base.hpp"
#include "holon/corelib/control/pd_ctrl/pd_ctrl_formula.hpp"
#include "holon/corelib/control/point_mass_model.hpp"
#include "holon/corelib/math/vec3d.hpp"

namespace holon {

template <typename State>
struct PdCtrlRefs {
  State position;
  State velocity;
  State stiffness;
  State damping;
};

template <typename State>
struct PdCtrlOutputs {
  State position;
  State velocity;
  State acceleration;
  State force;
};

template <
    typename State, typename StateArray = std::array<State, 2>,
    typename Solver = RungeKutta4<StateArray>,
    typename Data = PointMassModelData<State>,
    typename System = PointMassModelSystem<State>,
    typename Model = PointMassModel<State, StateArray, Solver, Data, System>,
    typename Refs = PdCtrlRefs<State>, typename Outputs = PdCtrlOutputs<State> >
class PdCtrl : public CtrlBase<State, Solver, Data, Model, Refs, Outputs> {
  using Self =
      PdCtrl<State, StateArray, Solver, Data, System, Model, Refs, Outputs>;
  using Base = CtrlBase<State, Solver, Data, Model, Refs, Outputs>;
  using Function = typename System::Function;

 public:
  PdCtrl() : PdCtrl(this->model()) {}
  explicit PdCtrl(const Model& t_model) : Base(t_model) {
    this->model().set_initial_position(this->states().position);
    resetRefs();
    this->model().setForceCallback(getForceFunction());
  }
  virtual ~PdCtrl() = default;

  virtual Self& reset() {
    this->model().reset();
    return resetRefs();
  }

  virtual Self& reset(const State& t_initial_position) {
    this->model().reset(t_initial_position);
    return resetRefs();
  }

  virtual State force(const State& t_position, const State& t_velocity,
                      const double /* t_time */) {
    return pd_ctrl_formula::computeDesForce(
        t_position, t_velocity, this->refs().position, this->refs().velocity,
        this->refs().stiffness, this->refs().damping);
  }

  virtual Function getForceFunction() {
    namespace pl = std::placeholders;
    return std::bind(&Self::force, this, pl::_1, pl::_2, pl::_3);
  }

  virtual bool update() override {
    if (!Base::update()) return false;
    updateOutputs();
    return true;
  }

  virtual bool update(double dt) override { return Base::update(dt); }

 private:
  Self& resetRefs() {
    this->refs_ptr()->position = this->states().position;
    this->refs_ptr()->velocity = this->states().velocity;
    return *this;
  }
  Self& updateOutputs() {
    this->outputs_ptr()->position = this->states().position;
    this->outputs_ptr()->velocity = this->states().velocity;
    this->outputs_ptr()->acceleration = this->states().acceleration;
    this->outputs_ptr()->force = this->states().force;
    return *this;
  }
};

}  // namespace holon

#endif  // HOLON_CONTROL_PD_CTRL_HPP_