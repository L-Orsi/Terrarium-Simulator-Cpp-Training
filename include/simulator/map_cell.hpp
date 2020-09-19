#pragma once
#include <iostream>
#include <array>
#include <memory>

namespace simulation {

class Bug; // Forward declaration.

/* Number of adjacent cells a single cell has. */
constexpr uint8_t ADJACENT_CELLS = 4; // Up, Right, Left, Down.

/** Represents a cell of a `bug world` Map. */
class MapCell {
 private:
  std::shared_ptr<Bug> bug_;
  std::array<const std::shared_ptr<MapCell>, ADJACENT_CELLS> adjacent_cells_;

 public:
  /** Helper class to hold adjacent cells references of a single cell. 
   * Pointers contained by this class may be null, which means that there's no adjacent cell in that direction.
   */
  struct AdjacentCellsReference {
    std::shared_ptr<MapCell> up = nullptr;
    std::shared_ptr<MapCell> down = nullptr;
    std::shared_ptr<MapCell> left = nullptr;
    std::shared_ptr<MapCell> right = nullptr;

    AdjacentCellsReference(
      const std::shared_ptr<MapCell> up, 
      const std::shared_ptr<MapCell> down, 
      const std::shared_ptr<MapCell> left,
      const std::shared_ptr<MapCell> right);
  
    AdjacentCellsReference(const AdjacentCellsReference& adjacent_cells_reference);
  };

  /** Constructs a cell for the simulation map
   * @param bug reference to the bug contained by this cell. Can be empty.
   * @param adjacent_cells references to adjacent cells.
   */
  MapCell(const std::shared_ptr<Bug> bug, const AdjacentCellsReference& adjacent_cells);

  /** Constructs a cell for the simulation map, with empty adjacent references.
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

  /** Moves the bug occupying this cell to another one. 
   * 
   * <p> This method replaces destination's bug for the one held by this class' instance. 
   *     In other words, if destination's bug reference is not empty, the caller must take into account
   *     that the bug reference held in destination will get lost.
   * <p> This cell's bug reference is always set to `nullptr` after the bug is moved.
  */
  void move_bug(const std::shared_ptr<MapCell> destination);

  /** Gets an array of adjacent cells to this cell. */
  const std::array<const std::shared_ptr<MapCell>, ADJACENT_CELLS>& get_adjacents() const;

  /** Gets a free adjacent cell if any. */
  const std::shared_ptr<MapCell> get_free_adjacent() const;

};

} // namespace

