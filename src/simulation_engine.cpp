#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <iterator>

#include <simulator/simulation_engine.hpp>
#include <simulator/simulation_map.hpp>
#include <simulator/bug_descriptor.hpp>
#include <simulator/bug.hpp>

namespace simulation {

const uint16_t ANTS = 10;
const uint16_t DOODLEBUGS = 3;

uint16_t n_rounds_ = 0;

SimulationEngine::SimulationEngine(): map_(std::make_shared<SimulationMap>()){}

void SimulationEngine::start_simulation(uint16_t n_rounds) {

  // Simulates rounds until either n_rounds are over or only specimens of one specie are left.
  for(n_rounds_=n_rounds;n_rounds_ && simulate_round();n_rounds_--);
}

bool SimulationEngine::simulate_round() {
  uint16_t species_left = 0;
  std::cout << "--------" << std::endl;
  std::cout << "[SimulationEngine] Round: " << n_rounds_ << std::endl;
  map_->print_map();
  
  // Iterates over cells and makes every bug move/breed/eat/starve accordingly.
  for(auto cell: map_->get_cells()) {
    auto bug_ref = cell->get_bug();
    if(bug_ref && !bug_ref->has_played()) {
      bug_ref->play_round();
    }
  }

  // Analyzes results of the rounds.
  bool only_ants = true;
  bool only_doo = true;
  for(auto cell: map_->get_cells()) {
    if(cell->get_bug()) {
      species_left++;
      only_doo &= !cell->get_bug()->is_prey();
      only_ants &= cell->get_bug()->is_prey();
      cell->get_bug()->reset_turn();
    }
  }

  // Checks whether it makes sense to continue the simulation.
  if(only_doo) {
    std::cout << "**Doodlebugs Won**" << std::endl;
    return false;
  }
  if(only_ants) {
    std::cout << "**Ants Won**" << std::endl;
    return false;
  }
  std::cout << "[SimulationEngine]" << "Species left in map: " << species_left << std::endl;
  std::cout << "--------" << std::endl;
  return true;
}

void SimulationEngine::populate_map_with_species() {
    std::random_device rd;
    std::mt19937 g(rd());
    auto cells = map_->get_cells();
    std::shuffle(cells.begin(), cells.end(), g);
    auto cells_it = cells.begin();
    for(auto i=0; i<ANTS; i++) {
      (*cells_it)->set_bug(std::make_shared<Bug>(*cells_it, BugDescriptorDatabase::get_ant_descriptor()));
      (*cells_it)->get_bug()->add_observer(this);
      if(++cells_it == cells.end()) {
        return;
      };
    }
    for(auto j=0; j<DOODLEBUGS; j++) {
      (*cells_it)->set_bug(std::make_shared<Bug>(*cells_it, BugDescriptorDatabase::get_doodlebug_descriptor()));
      (*cells_it)->get_bug()->add_observer(this);
      if(++cells_it == cells.end()) {
        return;
      }
    }
}

void SimulationEngine::on_bug_born(std::shared_ptr<Bug> b) {
  std::cout << "[SimulationEngine]" << b->get_name() << " was born." << std::endl;
  b->add_observer(this);
}
  
void SimulationEngine::on_bug_died(std::shared_ptr<Bug> b) {
  std::cout << "[SimulationEngine]" << b->get_name() << " died." << std::endl;
  b->get_cell()->set_bug(nullptr);
}

} // namespace: simulation

