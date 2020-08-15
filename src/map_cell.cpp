#include <iostream>
#include <array>
#include <memory>
#include <simulator/map_cell.hpp>
#include <simulator/bug.hpp>

namespace simulation
{

MapCell::MapCell(const std::shared_ptr<Bug> bug, const AdyacentCellsReference& c_ref) 
  : _bug(bug), _adyacent_cells{c_ref.up, c_ref.left, c_ref.down, c_ref.right} {}

MapCell::MapCell(const std::shared_ptr<Bug> bug) 
  : _bug(bug), _adyacent_cells{nullptr, nullptr, nullptr, nullptr} {}

std::shared_ptr<Bug> MapCell::get_bug() const {
  return _bug;
}

void MapCell::set_bug(const std::shared_ptr<Bug> bug) {
  _bug = bug;
}

const std::array<const std::shared_ptr<MapCell>, ADYACENT_CELLS>& MapCell::get_adyacents() const {
  return _adyacent_cells;
}

const std::shared_ptr<MapCell> MapCell::get_free_adyacent() const {
  for (auto adyacent_cell : _adyacent_cells) {
    if(!adyacent_cell->get_bug()){
      return adyacent_cell;
    }
  }
  return nullptr;
}

} // namespace: simulation

