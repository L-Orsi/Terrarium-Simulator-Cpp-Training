#include <memory>
#include <vector>

#include <simulator/bug.hpp>
#include <simulator/map_cell.hpp>

namespace simulation
{
  Bug::Bug(std::shared_ptr<MapCell> cell, const bool is_predator) : cell_(cell), is_prey_(!is_predator) {}

  bool Bug::is_dead() const {
    return !is_alive_;
  }

  bool Bug::is_prey() const {
    return is_prey_;
  }

  void Bug::play_round() const {
    
  }

  void Bug::get_eaten() {
    is_alive_ = false;
    notify_death(cell_->get_bug());
  }

  uint8_t Bug::get_rounds_to_breed() const {
    return DEFAULT_ROUNDS_TO_BREED;
  }

  uint8_t Bug::get_rounds_to_starve() const {
    return DEFAULT_ROUNDS_TO_STARVE;
  }

  void Bug::reset_starve_counter() {
    starve_counter_ = get_rounds_to_starve();
  }

  void Bug::reset_breed_counter() {
    breed_counter_ = get_rounds_to_breed();
  }

  void Bug::increase_lifecycle_counters() {
    breed_counter_++;
    starve_counter_++;
  }

  void Bug::eat_move() {
    for(auto adjacent : cell_->get_adjacents()) {
      if(adjacent && adjacent->get_bug() && adjacent->get_bug()->is_prey()) {
        notify_death(adjacent->get_bug());
        reset_starve_counter();
        cell_->move_bug(adjacent);
        cell_ = adjacent;
        return;        
      }
    }
    move();
  }

  void Bug::move() {
    auto free_adjacent = cell_->get_free_adjacent();
    if(!free_adjacent) {
      return;
    }
    cell_->move_bug(free_adjacent);
    cell_ = free_adjacent;
  }

  void Bug::breed() {
    if(breed_counter_ < get_rounds_to_breed()) {
      return;
    }
    auto free_adjacent = cell_->get_free_adjacent();
    if(!free_adjacent) {
      return;
    }
    std::shared_ptr<Bug> newborn_bug = std::make_shared<Bug>(free_adjacent);
    free_adjacent->set_bug(newborn_bug);
    notify_birth(newborn_bug);
  }

} // namespace: simulation
