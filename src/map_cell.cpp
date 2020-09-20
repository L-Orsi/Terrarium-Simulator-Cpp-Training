#include <iostream>
#include <array>
#include <memory>
#include <simulator/map_cell.hpp>
#include <simulator/bug.hpp>

namespace simulation
{

MapCell::AdjacentCellsReference::AdjacentCellsReference(
    const std::shared_ptr<MapCell> right_ref, 
    const std::shared_ptr<MapCell> up_ref, 
    const std::shared_ptr<MapCell> left_ref,
    const std::shared_ptr<MapCell> down_ref): right(right_ref), up(up_ref), left(left_ref), down(down_ref) {}

MapCell::AdjacentCellsReference::AdjacentCellsReference(const AdjacentCellsReference& c_ref) {
  this->up = c_ref.up;
  this->left = c_ref.left;
  this->down = c_ref.down;
  this->right = c_ref.right;
}

MapCell::MapCell(const std::shared_ptr<Bug> bug, const AdjacentCellsReference& c_ref) 
  : bug_(bug), adjacent_cells_{c_ref.right, c_ref.up, c_ref.left, c_ref.down} {}

MapCell::MapCell(const std::shared_ptr<Bug> bug) 
  : bug_(bug) {}

std::shared_ptr<Bug> MapCell::get_bug() const {
  return bug_;
}

void MapCell::set_bug(const std::shared_ptr<Bug> bug) {
  bug_ = bug;
}

void MapCell::move_bug(const std::shared_ptr<MapCell> destination) {
  destination->set_bug(bug_);
  bug_ = nullptr;
}

std::array<std::shared_ptr<MapCell>, ADJACENT_CELLS>& MapCell::get_adjacents() {
  return adjacent_cells_;
}

const std::shared_ptr<MapCell> MapCell::get_free_adjacent() {
  for (auto adjacent_cell : adjacent_cells_) {
    if(adjacent_cell && !adjacent_cell->get_bug()){
      return adjacent_cell;
    }
  }
  return nullptr;
}

void MapCell::set_adjacents(const AdjacentCellsReference& c_ref) {
  adjacent_cells_[0] = c_ref.right;
  adjacent_cells_[1] = c_ref.up;
  adjacent_cells_[2] = c_ref.left;
  adjacent_cells_[3] = c_ref.down;
}

} // namespace: simulation

