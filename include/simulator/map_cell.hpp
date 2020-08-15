#pragma once
#include <iostream>
#include <array>
#include <memory>

namespace simulation {

class Bug; // Forward declaration.

/* Number of adyacent cells a single cell has. */
constexpr uint8_t ADYACENT_CELLS = 4; // Up, Right, Left, Down.

/** Represents a cell of a `bug world` Map. */
class MapCell {
 private:
  std::shared_ptr<Bug> _bug;
  std::array<const std::shared_ptr<MapCell>, ADYACENT_CELLS> _adyacent_cells;

 public:
  /** Helper class to hold adyacent cells references of a single cell. 
   * Pointers contained by this class may be null, which means that there's no adyacent cell in that direction.
   */
  struct AdyacentCellsReference {
    std::shared_ptr<MapCell> up;
    std::shared_ptr<MapCell> down;
    std::shared_ptr<MapCell> left;
    std::shared_ptr<MapCell> right;
  };

  /** Constructs a cell for the simulation map
   * @param bug reference to the bug contained by this cell. Can be empty.
   * @param adyacent_cells references to adyacent cells.
   */
  MapCell(const std::shared_ptr<Bug> bug, const AdyacentCellsReference& adyacent_cells);

  /** Constructs a cell for the simulation map, with empty adyacent references.
   * @param bug reference to the bug contained by this cell. Can be empty.
   */
  MapCell(const std::shared_ptr<Bug> bug);
  
  /** Gets the bug occuping this cell.
   * @return reference to the bug if there's one, else, an empty reference.
   */
  std::shared_ptr<Bug> get_bug() const;

  /** Sets a bug in this cell.
   * If there's already an bug in this cell, it gets replaced. This will erase 
   * the reference to such bug.
   * 
   * @param bug reference to the bug that will occupy this cell.
   * 
   */
  void set_bug(const std::shared_ptr<Bug> bug);

  /** Gets an array of adyacent cells to this cell. */
  const std::array<const std::shared_ptr<MapCell>, ADYACENT_CELLS>& get_adyacents() const;

  /** Gets a free adyacent cell if any. */
  const std::shared_ptr<MapCell> get_free_adyacent() const;

};

} // namespace

