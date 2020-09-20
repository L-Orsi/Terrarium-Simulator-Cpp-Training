#pragma once
#include <unordered_set>
#include <memory>

#include "bug_observer.hpp"

namespace simulation {

class Bug;  // Forward declaration.

/** Notifies observers of Bug events.
 *
 * <ol> Relevant events are the following:
 *  <li> Bug death.
 *  <li> Bug breeding.
 * <ol/>
 */
class BugNotifier {
 private:
  std::unordered_set<BugObserver*> observers_;

 public:
  /** Creates an instance of a bug notifier. */
  BugNotifier();

  /** Adds an observer that will be notified when relevant events occur.
   *
   * <p>Adding twice the same observer has no effect.
   */
  void add_observer(BugObserver* observer);

  /** Removes an observer so that it is no longer notified of bug events.
   *
   * <p>Removing a non existent observer has no effect.
  */
  void remove_observer(BugObserver* observer);

  /** Notifies observer of bug birth.
   * @param newborn_bug reference to the newborn insect.
   */
  void notify_birth(std::shared_ptr<Bug> newborn_bug);

  /** Notifies observer of a bug death.
   * @param newborn_bug reference to the dead insect.
   */
  void notify_death(std::shared_ptr<Bug> dead_bug);
};
} // namespace: simulation.
