#pragma once

#include <memory>
#include <vector>

namespace simulation {

class Bug;  // Forward declaration.

/** To be implemented by classes interested in being notified of bug events. */
class BugObserver {
 public:
  /** Called when a bug is born.
   * @param newborn reference to the newly born bug.
   */
  virtual void on_bug_born(std::shared_ptr<Bug> newborn) = 0;

  /** Called when a bug dies.
   * @param dead_bug reference to the dead bug.
   */
  virtual void on_bug_died(std::shared_ptr<Bug> dead_bug) = 0;
};

}  // namespace: simulation
