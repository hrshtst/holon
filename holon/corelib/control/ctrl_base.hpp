/* ctrl_base - Base class of controller
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

#ifndef HOLON_CONTROL_CTRL_BASE_HPP_
#define HOLON_CONTROL_CTRL_BASE_HPP_

#include <memory>

namespace holon {

template <typename State, typename Solver, typename Data, typename Model,
          typename Refs, typename Outputs>
class CtrlBase {
 protected:
  using Self = CtrlBase<State, Solver, Data, Model, Refs, Outputs>;
  using States = Data;
  using StatesPtr = std::shared_ptr<States>;
  using RefsPtr = std::shared_ptr<Refs>;
  using OutputsPtr = std::shared_ptr<Outputs>;

 public:
  CtrlBase()
      : m_model(),
        m_states_ptr(m_model.data_ptr()),
        m_refs_ptr(std::make_shared<Refs>()),
        m_outputs_ptr(std::make_shared<Outputs>()) {}
  explicit CtrlBase(const Model& t_model) : CtrlBase() {
    *m_states_ptr = t_model.data();
  }
  virtual ~CtrlBase() = default;

  // accessors
  const Model& model() const { return m_model; }
  Model& model() { return m_model; }
  const States& states() const { return *m_states_ptr; }
  States& states() { return *m_states_ptr; }
  const Refs& refs() const { return *m_refs_ptr; }
  const Outputs& outputs() const { return *m_outputs_ptr; }
  const StatesPtr& states_ptr() const { return m_states_ptr; }
  const RefsPtr& refs_ptr() const { return m_refs_ptr; }
  const OutputsPtr& outputs_ptr() const { return m_outputs_ptr; }

  double time() const { return model().time(); }
  double time_step() const { return model().time_step(); }

  // mutators
  Self& set_states_ptr(StatesPtr t_states_ptr) {
    m_states_ptr = t_states_ptr;
    m_model.set_data_ptr(t_states_ptr);
    return *this;
  }
  Self& set_refs_ptr(RefsPtr t_refs_ptr) {
    m_refs_ptr = t_refs_ptr;
    return *this;
  }
  Self& set_outputs_ptr(OutputsPtr t_outputs_ptr) {
    m_outputs_ptr = t_outputs_ptr;
    return *this;
  }
  Self& set_time_step(double dt) {
    m_model.set_time_step(dt);
    return *this;
  }

  // update functions
  virtual bool update() { return m_model.update(); }
  virtual bool update(double dt) {
    this->set_time_step(dt);
    return this->update();
  }

 private:
  Model m_model;
  StatesPtr m_states_ptr;
  RefsPtr m_refs_ptr;
  OutputsPtr m_outputs_ptr;
};

}  // namespace holon

#endif  // HOLON_CONTROL_CTRL_BASE_HPP_