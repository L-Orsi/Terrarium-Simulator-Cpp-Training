#include <iostream>
#include <vector>
#include <memory>

#include <simulator/simulation_map.hpp>
#include <simulator/map_cell.hpp>
#include <simulator/bug.hpp>

namespace simulation {

SimulationMap::SimulationMap(uint8_t rows, uint8_t cols): rows_(rows), cols_(cols) {
  for(auto k=0; k<rows*cols; k++) {
    cells_.push_back(std::make_shared<MapCell>(nullptr));
  }
  for(auto i=0; i<rows; i++) {
    for(auto j=0; j<cols; j++) {
      cells_[i*cols+j]->set_adjacents(
        {
          (j+1 >= cols) ? nullptr : cells_[i*cols+(j+1)],
          (i-1 < 0) ? nullptr : cells_[(i-1)*cols+j],
          (j-1 < 0) ? nullptr : cells_[i*cols+(j-1)],
          (i+1 >= rows) ? nullptr : cells_[(i+1)*cols+(j)],
        }
      );
    }
  }
}

MapCellReferences& SimulationMap::get_cells() {
  return cells_;
}

void SimulationMap::print_map() {
    for(auto i=0; i<rows_; i++) {
      std::cout << std::endl;
      for(auto j=0; j<cols_; j++) {
        auto bg = cells_[i*cols_+j]->get_bug();
        if(!bg){
          std::cout << "[ ]";
        } else {
          std::cout << bg->get_name();
        }
          
        std::cout << "\t";
      }
    }
     std::cout << std::endl;
}

} // namespace: simulation

