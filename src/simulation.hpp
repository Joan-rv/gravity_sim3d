#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

#include "planet.hpp"

void sim_update(float dt, std::vector<Planet> &planets);

#endif
