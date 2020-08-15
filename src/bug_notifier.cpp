#include <set>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug.hpp>
#include <simulator/bug_observer.hpp>

namespace simulation 
{
  BugNotifier::BugNotifier() {}

  void BugNotifier::add_observer(std::shared_ptr<BugObserver> observer) {
    _observers.insert(observer);
  }

  void BugNotifier::remove_observer(std::shared_ptr<BugObserver> observer) {
    _observers.erase(observer);
  }

  void BugNotifier::notify_birth(std::shared_ptr<Bug> newborn_bug) {
    for(const auto observer : _observers) {
      observer->on_bug_born(newborn_bug);
    }
  }

  void BugNotifier::notify_death(std::shared_ptr<Bug> dead_bug) {
    for(const auto observer : _observers) {
      observer->on_bug_died(dead_bug);
    }
  }

}