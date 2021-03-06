// Copyright 2019 Carlos San Vicente
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/// \file
/// \brief This file provides an implementation for a Runge-Kutta method to solve
///        ordinary differential equations (ODE)

#ifndef PENDULUM_SIMULATION__RUNGE_KUTTA_HPP_
#define PENDULUM_SIMULATION__RUNGE_KUTTA_HPP_

#include <vector>
#include <stdexcept>
#include <functional>

namespace pendulum
{

using derivativeF = std::function<double (const std::vector<double> &, double, size_t)>;

/// \class This class implements a classic 4th order
/// <a href="https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods"> Runge Kutta method</a>
///
/// This method is based on the trapezoidal rule and it just allocates memory at initialization.
class RungeKutta
{
public:
  explicit RungeKutta(size_t dimension)
  : n(dimension)
  {
    k1.resize(dimension);
    k2.resize(dimension);
    k3.resize(dimension);
    k4.resize(dimension);
    state.resize(dimension);
  }

  /// \brief Time step using 4th-orderRunge Kutta and trapezoidal rule
  /// \param[in] df Derivative function pointing to the ODE equations to solve
  /// \param[in,out] y Status vector with the previous status at input and next state at output.
  /// \param[in] h Time step.
  /// \param[in] u Single input in the equations.
  /// \throw std::invalid_argument If the state vector doesn't has wrong dimensions.
  void step(derivativeF df, std::vector<double> & y, double h, double u)
  {
    // TODO(carlosvg) generalize this function for multiple inputs
    if (y.size() != n) {
      throw std::invalid_argument("wrong state size vector");
    }

    size_t i = 0;

    // stage 1
    for (i = 0; i < n; i++) {
      k1[i] = df(y, u, i);
    }

    // stage 2
    for (i = 0; i < n; i++) {
      state[i] = y[i] + h * 0.5 * k1[i];
    }
    for (i = 0; i < n; i++) {
      k2[i] = df(state, u, i);
    }

    // stage 3
    for (i = 0; i < n; i++) {
      state[i] = y[i] + h * (0.5 * k2[i]);
    }
    for (i = 0; i < n; i++) {
      k3[i] = df(state, u, i);
    }

    // stage 4
    for (i = 0; i < n; i++) {
      state[i] = y[i] + h * (1.0 * k3[i]);
    }
    for (i = 0; i < n; i++) {
      k4[i] = df(state, u, i);
    }

    // update next step
    for (i = 0; i < n; i++) {
      y[i] = y[i] + (h / 6.0) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
    }
  }

private:
  // state space vector
  // state[0]: cart position
  // state[1]: cart velocity
  // state[2]: pole position
  // state[3]: pole velocity
  std::vector<double> state;

  // space state dimension
  size_t n;

  // Runge-kutta increments
  std::vector<double> k1, k2, k3, k4;
};

}  // namespace pendulum

#endif  // PENDULUM_SIMULATION__RUNGE_KUTTA_HPP_
