#pragma once

#include <memory>
#include <vector>

#include "map_cell.hpp"
#include "bug_notifier.hpp"
#include "bug_observer.hpp"
#include "bug_descriptor.hpp"

namespace simulation
{

class MapCell; // Forward declaration.
class BugDescriptor; // Forward declaration.

/** A bug, which can perform the following actions:
 * <ol>
 *  <li> `Move` to an empty cell, or move to an occupied cell if the bug is a predator.
 *  <li> `Breed` to an empty cell if the bug is ready to breed.
 *  <li> `Die`, if the starves or it is eaten by a predator.
 * </ol>
 * 
 * <p>This class inherits from BugNotifier class, which allows observer classes to register
 *    to bug events.
 */
class Bug : public BugNotifier {
 private:    
  /** References. */
  std::shared_ptr<MapCell> cell_;

  /** Lifecycle constants. */
  uint16_t rounds_to_breed_;
  uint16_t rounds_to_starve_;
  bool _played = false;

  /** Lifecycle counters. */
  uint16_t breed_counter_ = 0;
  uint16_t starve_counter_ = 0;

  /** Lifecycle flags. */
  bool is_prey_;
  bool is_alive_ = true;

  /** Name. */
  std::string name_;

 public:
  /** Default values. */
  static const uint16_t DEFAULT_ROUNDS_TO_BREED = 3;
  static const uint16_t DEFAULT_ROUNDS_TO_STARVE = 3;
    
  /** Creates a Bug.
   * @param cell map cell where the bug will be placed.
   * @param is_predator whether the bug is a prey or a predator.
   */
    /** Creates a Bug based on a descriptor's specifications. */
  Bug(std::shared_ptr<MapCell> cell, const bool is_predator=false, 
    const uint16_t rounds_to_breed=DEFAULT_ROUNDS_TO_STARVE,
    const uint16_t rounds_to_starve=DEFAULT_ROUNDS_TO_STARVE,
    const std::string name="GenericBug");

  /** Creates a Bug based on a descriptor's specifications. */
  Bug(std::shared_ptr<MapCell> cell, const BugDescriptor& bug_descriptor);

  /** Gets the bug name. */
  std::string get_name() const;

  /** Gets the cell where the bug is at. */
  std::shared_ptr<MapCell> get_cell() {
    return cell_;
  }

  void reset_turn();

  bool has_played() const;

  /** Returns true if the bug is dead. */
  bool is_dead() const;

  /** Returns true if the bug can be predated. */
  bool is_prey() const;

  /** Runs the different lifecycle actions related to this bug
   * A base bug does the following in the listed order:
   * <ol>
   *   <li> move (or eat_move if it's a predator).
   *   <li> breed (it it's ready to do so).
   *   <li> starve (if it spent too many rounds without eating being a predator).
   * </ol>
   */
  void play_round();

  /** Get eaten (by a predator).
   * 
   *  <p>Does nothing if the bug is not a prey.
   */
  void get_eaten();

  /** Marks a prey in an adjacent cell as dead and moves to its place. If there's no prey, it just moves.
   * 
   * <p>This action resets the #_starve_counter if a prey is eaten. 
   */
  void eat_move();

  /** Starves if bug hasn't eaten after a fixed amount of rounds, unless it can't starve. */
  void starve();

  /** Moves to a free adyacent cell. */
  void move();

  /** Breeds to a free adjacent cell.
   * 
   * <p>If no adjacent cell is free, breeding is postponed until there's room to do so.
   * 
   * <p>Breeding resets the breed counter.
   */
  void breed();

  /** Resets the starve counter. */
  void reset_starve_counter();

  /** Resets the breed counter. */
  void reset_breed_counter();

  /** Increases both breed and starve counters. */
  void increase_lifecycle_counters();

 protected: 

  /** Gets the amount of rounds that are needed for this bug to breed. */
  uint16_t get_rounds_to_breed() const;

  /** Gets the number of rounds that are needed for this bug to starve (die) if it doesn't eat_move. 
   * <p> This method may return 0, which means that the bug cannot starve.
   */
  uint16_t get_rounds_to_starve() const;
};

} // namespace: simulation
