#include <memory>
#include <vector>

#include <simulator/bug.hpp>
#include <simulator/map_cell.hpp>

namespace simulation
{
  Bug::Bug(std::shared_ptr<MapCell> cell, const BugDescriptor& descriptor)
  : cell_(cell), rounds_to_breed_(descriptor.rounds_to_breed), 
    rounds_to_starve_(descriptor.rounds_to_starve), is_prey_(!descriptor.is_predator),name_(descriptor.bug_name) {}

  Bug::Bug(std::shared_ptr<MapCell> cell, const bool is_predator, 
    const uint16_t rounds_to_breed,
    const uint16_t rounds_to_starve,
    const std::string name) 
   : cell_(cell), rounds_to_breed_(rounds_to_breed), 
     rounds_to_starve_(rounds_to_starve), is_prey_(!is_predator),name_(name) {}

  std::shared_ptr<MapCell> Bug::get_cell() const {
    return cell_;
  }

  bool Bug::is_dead() const {
    return !is_alive_;
  }

  bool Bug::is_prey() const {
    return is_prey_;
  }

  void Bug::play_round() {
   breed();
   is_prey_ ? move() : eat_move();
   starve();
   _played = true;
   increase_lifecycle_counters();
  }

  void Bug::reset_turn() {
    _played = false;
  }

  void Bug::starve() {
    if(rounds_to_starve_ == starve_counter_) {
      std::cout << "[" << name_ << "]" << "starved." << std::endl;
      is_alive_ = false;
      notify_death(cell_->get_bug());
    }
  }

  void Bug::get_eaten() {
    is_alive_ = false;
    notify_death(cell_->get_bug());
  }

  uint16_t Bug::get_rounds_to_breed() const {
    return rounds_to_breed_;
  }

  uint16_t Bug::get_rounds_to_starve() const {
    return rounds_to_starve_;
  }

  void Bug::reset_starve_counter() {
    starve_counter_ = 0;
  }

  void Bug::reset_breed_counter() {
    breed_counter_ = 0;
  }

  void Bug::increase_lifecycle_counters() {
    breed_counter_++;
    starve_counter_++;
  }

  void Bug::eat_move() {
    for(auto adjacent : cell_->get_adjacents()) {
      if(adjacent && adjacent->get_bug() && adjacent->get_bug()->is_prey()) {
        std::cout << "[" << name_ << "]" << "ate" << std::endl;
        notify_death(adjacent->get_bug());
        reset_starve_counter();
        cell_->move_bug(adjacent);
        cell_ = adjacent;
        return;        
      }
    }
    move();
  }

  bool Bug::has_played() const {
    return _played;
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
      breed_counter_--;
      return;
    }
    breed_counter_ = 0;
    std::shared_ptr<Bug> newborn_bug = std::make_shared<Bug>(free_adjacent, !is_prey_, rounds_to_breed_, rounds_to_starve_, name_ + ".");
    free_adjacent->set_bug(newborn_bug);
    std::cout << "[" << name_ << "]" << "gave birth." << std::endl;
    notify_birth(newborn_bug);
  }

  std::string Bug::get_name() const {
    return name_;
  }

} // namespace: simulation
