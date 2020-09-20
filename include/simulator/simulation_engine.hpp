#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "simulation_map.hpp"
#include "bug_observer.hpp"

namespace simulation {

constexpr uint8_t ANTS_INIT_NUMBER = 5;
constexpr uint8_t DOODLEBUGS_INIT_NUMBER = 2;

/** Owner of cells that can be occupied by species in a simulation. 
 * 
 * This class is in charge of creating and linking all the cells in the simulation.
 */
class SimulationEngine : BugObserver {
 private:
  const std::shared_ptr<SimulationMap> map_;

 public:
  SimulationEngine();

  /** Starts the simulation.
   * 
   * <ol>Simulation runs until either :
   *   <li> The biodiversity of the ecosystem equals 1.
   *   <li> The max amount of rounds is reached.
   * 
   * @param n_rounds: the number of rounds to be simulated. 
   *                  The actual number of rounds ran may be lower if there're
   *                  only bugs of a single specie left.
   */
  void start_simulation(uint16_t n_rounds);

  /** Executes Bug#play_round for each Bug in the Map. 
   * 
   * @return false if there's only one kind of bugs left, true otherwise.
   * 
  */
  bool simulate_round();

  /** Initializes the simulation Map with bugs randomly. */
  void populate_map_with_species();

  /** Registers the birth of a bug. */
  void on_bug_born(std::shared_ptr<Bug> newborn) override;
  
  /** Registers the death of a bug and removes it from the map. */
  void on_bug_died(std::shared_ptr<Bug> dead_bug) override;
};

} // namespace: simulation

