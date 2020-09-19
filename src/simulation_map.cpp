#include <iostream>
#include <array>
#include <memory>

#include <simulator/simulation_map.hpp>
#include <simulator/map_cell.hpp>

namespace simulation {

SimulationMap::SimulationMap() {}

std::array<const std::shared_ptr<MapCell>, CELLS_TOTAL>& SimulationMap::get_cells() {
  return cells_;
}

} // namespace: simulation

