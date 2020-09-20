#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug_observer.hpp>
#include <simulator/bug.hpp>

#include "mocks/bug_observer_mock.h"

TEST(Bug, Move_SuccessfullyMovesToEmpty) {  
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->move();

  EXPECT_EQ((cells_refs.up->get_bug()), bug_under_test);
  EXPECT_EQ((test_cell_ref->get_bug()), nullptr);
}

TEST(Bug, Move_NoFreeadjacent) {  
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->move();

  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
}

TEST(Bug, EatMove_Successful) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<BugObserverMock>();

  std::shared_ptr<simulation::Bug> predator(std::make_shared<simulation::Bug>(nullptr, /*is_predator=*/ true));
  std::shared_ptr<simulation::Bug> prey(std::make_shared<simulation::Bug>(nullptr, /*is_predator=*/ false));
  
  EXPECT_CALL(*std::dynamic_pointer_cast<BugObserverMock>(testable_observer), on_bug_died(prey)).Times(1);

  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(predator), // Can't eat! It's a predator.
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(prey), // It's a prey! Will eat.
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer.get());
  bug_under_test->eat_move();

  EXPECT_EQ((cells_refs.left->get_bug()), bug_under_test);
  EXPECT_EQ((test_cell_ref->get_bug()), nullptr);
}

TEST(Bug, EatMove_adjacentsClearSoSimpleMove) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<BugObserverMock>();
  
  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer.get());
  bug_under_test->eat_move();

  EXPECT_EQ((cells_refs.right->get_bug()), bug_under_test);
  EXPECT_EQ((test_cell_ref->get_bug()), nullptr);
}

TEST(Bug, Breed_adjacentsClearBreedsSuccessfully) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<BugObserverMock>();

  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };
  
  EXPECT_CALL(*std::dynamic_pointer_cast<BugObserverMock>(testable_observer), on_bug_born).Times(1);

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer.get());
  for(auto i=0; i < bug_under_test->DEFAULT_ROUNDS_TO_BREED; i++) {
    bug_under_test->increase_lifecycle_counters(); // Advance time until the bug is ready to breed.
  }
  bug_under_test->breed();
  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
}

TEST(Bug, Breed_PostponedDueToOccupiedCells) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<BugObserverMock>();

  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));
  
  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer.get());
  for(auto i=0; i < bug_under_test->DEFAULT_ROUNDS_TO_BREED; i++) {
    bug_under_test->increase_lifecycle_counters(); // Advance time until the bug is ready to breed.
  }
  bug_under_test->breed(); // Doesn't breed -> there's nowhere to!

  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
}

TEST(Bug, Breed_DontBreedIfNotReady) {  
  std::shared_ptr<simulation::BugObserver> testable_observer = std::make_shared<BugObserverMock>();

  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };

  std::shared_ptr<simulation::MapCell> test_cell_ref = std::make_shared<simulation::MapCell>(nullptr, cells_refs);
  std::shared_ptr<simulation::Bug> bug_under_test = std::make_shared<simulation::Bug>(test_cell_ref, false);

  test_cell_ref->set_bug(bug_under_test);
  bug_under_test->add_observer(testable_observer.get());
  bug_under_test->breed();

  EXPECT_EQ((cells_refs.up->get_bug()), nullptr);
  EXPECT_EQ((test_cell_ref->get_bug()), bug_under_test);
}

