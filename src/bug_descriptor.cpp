#include <simulator/bug_descriptor.hpp>

namespace simulation {

static uint8_t ant_name_count = 0;
static uint8_t doo_name_count = 0;

BugDescriptor BugDescriptorDatabase::get_ant_descriptor() {
  ant_name_count++;
  return {false, 3, 100, "A" + std::to_string(ant_name_count)};
}

BugDescriptor BugDescriptorDatabase::get_doodlebug_descriptor() {
  doo_name_count++;
  return {true, 8, 3,  "D" + std::to_string(doo_name_count)};
}

}
