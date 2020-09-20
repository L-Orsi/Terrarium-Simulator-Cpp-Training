#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "bug.hpp"
#include "map_cell.hpp"

namespace simulation {

/** Contains information required to create a bug. */
struct BugDescriptor {
  bool is_predator;
  uint8_t rounds_to_breed;
  uint8_t rounds_to_starve;
  std::string bug_name;
};

class BugDescriptorDatabase {
 public:
  static BugDescriptor get_ant_descriptor();
  static BugDescriptor get_doodlebug_descriptor();
};

}
