#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "map_cell.hpp"

namespace simulation {

using MapCellReferences = std::vector<std::shared_ptr<MapCell>>;

/* Map default dimensions. */
constexpr uint8_t ROWS_DEFAULT = 9;
constexpr uint8_t COLS_DEFAULT = 9;
constexpr uint16_t CELLS_TOTAL = ROWS_DEFAULT * COLS_DEFAULT;  

/** Owner of cells that can be occupied by species in a simulation. 
 * 
 * This class is in charge of creating and linking all the cells in the simulation.
 */
class SimulationMap {
 private:
  const uint8_t rows_;
  const uint8_t cols_;
  MapCellReferences cells_;

 public:
  /** Creates the map where the simulation will be run. 
   * 
   * This means both (1) creating all the cells and (2) linking them.
   * @param rows number of rows.
   * @param cols number of columns.
   */
  SimulationMap(uint8_t rows=ROWS_DEFAULT, uint8_t cols=COLS_DEFAULT);

  /** Prints map in console. */
  void print_map();
  
  /** Gets all the cells in the map. */
  MapCellReferences& get_cells();
};

} // namespace: simulation

