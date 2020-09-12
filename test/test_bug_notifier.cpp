#include "gtest/gtest.h"
#include <stdexcept>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug_observer.hpp>
#include <simulator/bug.hpp>

class TestableObserver : public simulation::BugObserver {
  public:
    bool on_bug_born_called_;
    bool on_bug_died_called_;

    TestableObserver() : on_bug_born_called_(false), on_bug_died_called_(false) {}
  
    void on_bug_born(std::shared_ptr<simulation::Bug>) {
      on_bug_born_called_ = true;
    };

    void on_bug_died(std::shared_ptr<simulation::Bug>) {
      on_bug_died_called_ = true;
    };
};

TEST(BugNotifier, NotifyBirth_SuccessfullyNotifiesBirth) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserver>();
  test_notifier.add_observer(testable_observer);
  test_notifier.notify_birth(std::shared_ptr<simulation::Bug>{});
  EXPECT_TRUE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->on_bug_born_called_);
}


TEST(BugNotifier, NotifyDeath_SuccessfullyNotifiesDeath) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserver>();
  test_notifier.add_observer(testable_observer);
  test_notifier.notify_death(std::shared_ptr<simulation::Bug>{});
  EXPECT_FALSE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->on_bug_born_called_);
  EXPECT_TRUE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->on_bug_died_called_);
}

TEST(BugNotifier, RemoveObserver_UnregisterdObserverDoesNotReceiveEvents) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserver>();
  test_notifier.add_observer(testable_observer);
  test_notifier.remove_observer(testable_observer);
  test_notifier.notify_death(std::shared_ptr<simulation::Bug>{});
  EXPECT_FALSE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->on_bug_born_called_);
  EXPECT_FALSE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->on_bug_died_called_);
}


