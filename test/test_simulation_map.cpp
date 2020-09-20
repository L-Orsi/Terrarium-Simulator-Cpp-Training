#include "gtest/gtest.h"
#include <stdexcept>
#include <memory>

#include <simulator/map_cell.hpp>
#include <simulator/simulation_map.hpp>
#include <simulator/bug.hpp>

TEST(SimulationMap, SimulationMap_SuccessCreatesAndLinksMapCells) {
  simulation::SimulationMap map_under_test{3, 3};
  const uint8_t left_upper_corner_ind = 0;
  const uint8_t right_upper_corner_ind = 2;
  const uint8_t left_bottom_corner_ind = 6;
  const uint8_t right_bottom_corner_ind = 8;
  const uint8_t center_ind = 4;

  auto cells = map_under_test.get_cells();

  auto left_upper_adjacents = cells[left_upper_corner_ind]->get_adjacents();
  EXPECT_EQ(left_upper_adjacents[0], cells[1]);
  EXPECT_EQ(left_upper_adjacents[1], nullptr);
  EXPECT_EQ(left_upper_adjacents[2], nullptr);
  EXPECT_EQ(left_upper_adjacents[3], cells[3]);

  auto right_upper_adjacents = cells[right_upper_corner_ind]->get_adjacents();
  EXPECT_EQ(right_upper_adjacents[0], nullptr);
  EXPECT_EQ(right_upper_adjacents[1], nullptr);
  EXPECT_EQ(right_upper_adjacents[2], cells[1]);
  EXPECT_EQ(right_upper_adjacents[3], cells[5]);

  auto left_bottom_adjacents = cells[left_bottom_corner_ind]->get_adjacents();
  EXPECT_EQ(left_bottom_adjacents[0], cells[7]);
  EXPECT_EQ(left_bottom_adjacents[1], cells[3]);
  EXPECT_EQ(left_bottom_adjacents[2], nullptr);
  EXPECT_EQ(left_bottom_adjacents[3], nullptr);

  auto right_bottom_adjacents = cells[right_bottom_corner_ind]->get_adjacents();
  EXPECT_EQ(right_bottom_adjacents[0], nullptr);
  EXPECT_EQ(right_bottom_adjacents[1], cells[5]);
  EXPECT_EQ(right_bottom_adjacents[2], cells[7]);
  EXPECT_EQ(right_bottom_adjacents[3], nullptr);

  auto center_adjacents = cells[center_ind]->get_adjacents();
  EXPECT_EQ(center_adjacents[0], cells[5]);
  EXPECT_EQ(center_adjacents[1], cells[1]);
  EXPECT_EQ(center_adjacents[2], cells[3]);
  EXPECT_EQ(center_adjacents[3], cells[7]);

}
