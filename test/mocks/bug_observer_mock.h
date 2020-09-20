#include "gmock/gmock.h"
#include <simulator/bug_observer.hpp>

class BugObserverMock : public simulation::BugObserver {
  public:
    MOCK_METHOD(void, on_bug_born, (std::shared_ptr<simulation::Bug>), (override));
    MOCK_METHOD(void, on_bug_died, (std::shared_ptr<simulation::Bug>), (override));
};
