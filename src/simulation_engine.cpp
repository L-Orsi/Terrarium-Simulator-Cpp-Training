#include <iostream>
#include <vector>
#include <memory>

#include <simulator/simulation_engine.hpp>
#include <simulator/simulation_map.hpp>

namespace simulation {

SimulationEngine::SimulationEngine() {}

void SimulationEngine::simulate_round() {}

void SimulationEngine::populate_map_with_species() {}

void SimulationEngine::on_bug_born(std::shared_ptr<Bug>) {}
  
void SimulationEngine::on_bug_died(std::shared_ptr<Bug>) {}

} // namespace: simulation

