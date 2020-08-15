#include <memory>
#include <vector>

#include <simulator/bug.hpp>
#include <simulator/map_cell.hpp>

namespace simulation
{
  Bug::Bug(std::shared_ptr<MapCell> cell, const bool is_predator) : _cell(cell), _is_prey(!is_predator) {}

  bool Bug::is_dead() const {
    return !_is_alive;
  }

  bool Bug::is_prey() const {
    return _is_prey;
  }

  void Bug::play_round() const {
    
  }

  void Bug::get_eaten() {
    _is_alive = false;
    notify_death(_cell->get_bug());
  }

  uint8_t Bug::get_rounds_to_breed() const {
    return DEFAULT_ROUNDS_TO_BREED;
  }

  uint8_t Bug::get_rounds_to_starve() const {
    return DEFAULT_ROUNDS_TO_STARVE;
  }

  void Bug::reset_starve_counter() {
    _starve_counter = get_rounds_to_starve();
  }

  void Bug::reset_breed_counter() {
    _breed_counter = get_rounds_to_breed();
  }

  void Bug::increase_lifecycle_counters() {
    _breed_counter++;
    _starve_counter++;
  }

  void Bug::eat_move() {
    for(auto adyacent : _cell->get_adyacents()) {
      if(adyacent && adyacent->get_bug() && adyacent->get_bug()->is_prey()) {
        notify_death(adyacent->get_bug());
        reset_starve_counter();
        adyacent->set_bug(_cell->get_bug());
        _cell->set_bug(nullptr);
        _cell = adyacent;
        return;        
      }
    }
    move();
  }

  void Bug::move() {
    auto free_adyacent = _cell->get_free_adyacent();
    if(!free_adyacent) {
      return;
    }
    free_adyacent->set_bug(_cell->get_bug());
    _cell->set_bug(nullptr);
    _cell = free_adyacent;
  }

  void Bug::breed() {
    if(_breed_counter < get_rounds_to_breed()) {
      return;
    }
    auto free_adyacent = _cell->get_free_adyacent();
    if(!free_adyacent) {
      return;
    }
    std::shared_ptr<Bug> newborn_bug = std::make_shared<Bug>(free_adyacent);
    free_adyacent->set_bug(newborn_bug);
    notify_birth(newborn_bug);
  }

} // namespace: simulation
