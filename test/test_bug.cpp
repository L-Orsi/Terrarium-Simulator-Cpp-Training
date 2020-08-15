#include "gtest/gtest.h"
#include <stdexcept>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug_observer.hpp>
#include <simulator/bug.hpp>

class TestableObserverBug : public simulation::BugObserver {
  public:
    std::shared_ptr<simulation::Bug> cached_bug;

    TestableObserverBug() {}
  
    void on_bug_born(std::shared_ptr<simulation::Bug> bug) {
      cached_bug = bug;
    };

    void on_bug_died(std::shared_ptr<simulation::Bug> bug) {
      cached_bug = bug;
    };
};

TEST(Bug, Move_SuccessfullyMovesToEmpty) {  
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->move();

  EXPECT_EQ((cells_refs.down->get_bug()), bug_under_test);
  EXPECT_EQ((test_cell_ref->get_bug()), nullptr);
}

TEST(Bug, Move_NoFreeAdyacent) {  
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->move();

  EXPECT_EQ((cells_refs.up->get_bug()), non_empty_bug_2);
  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
}

TEST(Bug, EatMove_Successfull) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserverBug>();

  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr, /*is_predator=*/ false));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr, /*is_predator=*/ true));
  
  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2), // Can't eat! It's a predator.
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(non_empty_bug_1), // It's a prey! Will eat.
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer);
  bug_under_test->eat_move();

  EXPECT_EQ((cells_refs.left->get_bug()), bug_under_test);
  EXPECT_EQ((test_cell_ref->get_bug()), nullptr);
  EXPECT_EQ((std::dynamic_pointer_cast<TestableObserverBug>(testable_observer))->cached_bug, non_empty_bug_1);
}

TEST(Bug, EatMove_AdyacentsClearSoSimpleMove) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserverBug>();
  
  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer);
  bug_under_test->eat_move();

  EXPECT_EQ((cells_refs.up->get_bug()), bug_under_test);
  EXPECT_EQ((test_cell_ref->get_bug()), nullptr);
  EXPECT_EQ((std::dynamic_pointer_cast<TestableObserverBug>(testable_observer))->cached_bug, nullptr);
}

TEST(Bug, Breed_AdyacentsClearBreedsSuccessfully) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserverBug>();

  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer);
  for(auto i=0; i < bug_under_test->DEFAULT_ROUNDS_TO_BREED; i++) {
    bug_under_test->increase_lifecycle_counters(); // Advance time until the bug is ready to breed.
  }
  bug_under_test->breed();

  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
  EXPECT_EQ((std::dynamic_pointer_cast<TestableObserverBug>(testable_observer))->cached_bug, cells_refs.left->get_bug());
}

TEST(Bug, Breed_PostponedDueToOccupiedCells) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserverBug>();

  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer);
  for(auto i=0; i < bug_under_test->DEFAULT_ROUNDS_TO_BREED; i++) {
    bug_under_test->increase_lifecycle_counters(); // Advance time until the bug is ready to breed.
  }
  bug_under_test->breed(); // Doesn't breed -> there's nowhere to!

  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
  EXPECT_EQ((std::dynamic_pointer_cast<TestableObserverBug>(testable_observer))->cached_bug, nullptr);
}

TEST(Bug, Breed_DontBreedIfNotReady) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<TestableObserverBug>();

  simulation::MapCell::AdyacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer);
  bug_under_test->breed();

  EXPECT_EQ((cells_refs.up->get_bug()), nullptr);
  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
  EXPECT_EQ((std::dynamic_pointer_cast<TestableObserverBug>(testable_observer))->cached_bug, nullptr);
}






