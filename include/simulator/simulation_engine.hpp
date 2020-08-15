#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "simulation_map.hpp"
#include "bug_observer.hpp"

namespace simulation {

constexpr uint8_t ANTS_INIT_NUMBER = 10;
constexpr uint8_t DOODLEBUGS_INIT_NUMBER = 5;

/** Owner of cells that can be occupied by species in a simulation. 
 * 
 * This class is in charge of creating and linking all the cells in the simulation.
 */
class SimulationEngine : BugObserver {
 private:
  const SimulationMap _map;

 public:
  SimulationEngine();

  /** Executs Bug#play_round for each Bug in the Map. */
  void simulate_round();

  /** Initializes the simulation Map with bugs randomly. */
  void populate_map_with_species();

  /** Registers the birth of a bug. */
  void on_bug_born(std::shared_ptr<Bug> newborn) override;
  
  /** Registers the death of a bug and removes it from the map. */
  void on_bug_died(std::shared_ptr<Bug> dead_bug) override;
};

} // namespace: simulation

