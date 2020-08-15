#pragma once

#include <iostream>
#include <array>
#include <memory>

#include "map_cell.hpp"

namespace simulation {

/* Map default dimensions. */
constexpr uint8_t ROWS_DEFAULT = 20;
constexpr uint8_t COLS_DEFAULT = 20;
constexpr uint16_t CELLS_TOTAL = ROWS_DEFAULT * COLS_DEFAULT;  

/** Owner of cells that can be occupied by species in a simulation. 
 * 
 * This class is in charge of creating and linking all the cells in the simulation.
 */
class SimulationMap {
 private:
  std::array<const std::shared_ptr<MapCell>, CELLS_TOTAL> _cells;

 public:
  /** Creates the map where the simulation will be run. 
   * 
   * This means both (1) creating all the cells and (2) linking them.
   */
  SimulationMap();
  
  /** Gets all the cells in the map. */
  std::array<const std::shared_ptr<MapCell>, CELLS_TOTAL>& get_cells();
};

} // namespace: simulation

