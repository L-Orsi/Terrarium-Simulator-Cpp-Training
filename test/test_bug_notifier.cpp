#include "gtest/gtest.h"
#include <stdexcept>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug_observer.hpp>
#include <simulator/bug.hpp>

class TestableObserver : public simulation::BugObserver {
  public:
    bool _on_bug_born_called;
    bool _on_bug_died_called;

    TestableObserver() : _on_bug_born_called(false), _on_bug_died_called(false) {}
  
    void on_bug_born(std::shared_ptr<simulation::Bug>) {
      _on_bug_born_called = true;
    };

    void on_bug_died(std::shared_ptr<simulation::Bug>) {
      _on_bug_died_called = true;
    };
};

TEST(BugNotifier, NotifyBirth_SuccessfullyNotifiesBirth) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserver>();
  test_notifier.add_observer(testable_observer);
  test_notifier.notify_birth(std::shared_ptr<simulation::Bug>{});
  EXPECT_TRUE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->_on_bug_born_called);
}


TEST(BugNotifier, NotifyDeath_SuccessfullyNotifiesDeath) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserver>();
  test_notifier.add_observer(testable_observer);
  test_notifier.notify_death(std::shared_ptr<simulation::Bug>{});
  EXPECT_FALSE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->_on_bug_born_called);
  EXPECT_TRUE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->_on_bug_died_called);
}

TEST(BugNotifier, RemoveObserver_UnregisterdObserverDoesNotReceiveEvents) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserver>();
  test_notifier.add_observer(testable_observer);
  test_notifier.remove_observer(testable_observer);
  test_notifier.notify_death(std::shared_ptr<simulation::Bug>{});
  EXPECT_FALSE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->_on_bug_born_called);
  EXPECT_FALSE(std::dynamic_pointer_cast<TestableObserver>(testable_observer)->_on_bug_died_called);
}


