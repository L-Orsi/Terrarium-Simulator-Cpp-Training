#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug_observer.hpp>
#include <simulator/bug.hpp>

#include "mocks/bug_observer_mock.h"


TEST(BugNotifier, NotifyBirth_SuccessfullyNotifiesBirth) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::Bug> test_bug{};
  BugObserverMock testable_observer;
  EXPECT_CALL(testable_observer, on_bug_born(test_bug)).Times(1);
  EXPECT_CALL(testable_observer, on_bug_died(test_bug)).Times(0);
  test_notifier.add_observer(&testable_observer);
  test_notifier.notify_birth(test_bug);
}

TEST(BugNotifier, NotifyBirth_SuccessfullyNotifiesDeath) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::Bug> test_bug{};
  BugObserverMock testable_observer;
  EXPECT_CALL(testable_observer, on_bug_born(test_bug)).Times(0);
  EXPECT_CALL(testable_observer, on_bug_died(test_bug)).Times(1);
  test_notifier.add_observer(&testable_observer);
  test_notifier.notify_death(test_bug);
}



TEST(BugNotifier, RemoveObserver_UnregisterdObserverDoesNotReceiveEvents) {
  simulation::BugNotifier test_notifier = simulation::BugNotifier();
  std::shared_ptr<simulation::Bug> test_bug{};
  BugObserverMock testable_observer;
  EXPECT_CALL(testable_observer, on_bug_born(test_bug)).Times(0);
  EXPECT_CALL(testable_observer, on_bug_died(test_bug)).Times(0);
  test_notifier.add_observer(&testable_observer);
  test_notifier.remove_observer(&testable_observer);
  test_notifier.notify_death(test_bug);
}


