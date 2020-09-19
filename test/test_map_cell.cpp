#include "gtest/gtest.h"
#include <stdexcept>
#include <memory>

#include <simulator/bug_notifier.hpp>
#include <simulator/bug_observer.hpp>
#include <simulator/bug.hpp>


TEST(MapCell, GetSetBug_SuccessfullyGetsSetsBugReferences) {
  simulation::MapCell test_cell = simulation::MapCell(nullptr);
  std::shared_ptr<simulation::Bug> shared(std::make_shared<simulation::Bug>(nullptr));
  test_cell.set_bug(shared);
  EXPECT_EQ(test_cell.get_bug(), shared);
}

TEST(MapCell, Getadjacents_SuccessfullyGetsadjacents) {

  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));

  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(nullptr),
  };
  simulation::MapCell test_cell = simulation::MapCell(nullptr, cells_refs);

  uint8_t non_empty_bug_count = 0;
  for(auto cell : test_cell.get_adjacents()) {
      if(cell->get_bug()) {
          non_empty_bug_count++;
      }
  }
  EXPECT_EQ(non_empty_bug_count, 2);
}

TEST(MapCell, GetFreeadjacent_SuccessfullyGetsFirstFreeadjacent) {
      
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));

  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(nullptr),
  };
  simulation::MapCell test_cell = simulation::MapCell(nullptr, cells_refs);

  EXPECT_EQ(test_cell.get_free_adjacent(), cells_refs.down);
}

TEST(MapCell, GetFreeadjacent_GetsNullReferenceIfNoneEmpty) {      
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));

  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
      std::make_shared<simulation::MapCell>(non_empty_bug_1),
  };
  simulation::MapCell test_cell = simulation::MapCell(nullptr, cells_refs);

  EXPECT_EQ(test_cell.get_free_adjacent(), nullptr);
}

TEST(MapCell, MoveBug_BugMovedSuccessfully) {
  std::shared_ptr<simulation::Bug> non_empty_bug_1(std::make_shared<simulation::Bug>(nullptr));
  std::shared_ptr<simulation::Bug> non_empty_bug_2(std::make_shared<simulation::Bug>(nullptr));

  simulation::MapCell::AdjacentCellsReference cells_refs = {
      std::make_shared<simulation::MapCell>(non_empty_bug_2),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
      std::make_shared<simulation::MapCell>(nullptr),
  };
  simulation::MapCell test_cell = simulation::MapCell(non_empty_bug_1, cells_refs);
  test_cell.move_bug(cells_refs.left);
  EXPECT_EQ(test_cell.get_bug(), nullptr);
  EXPECT_EQ(cells_refs.left->get_bug(), non_empty_bug_1);
}




