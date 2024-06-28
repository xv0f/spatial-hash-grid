#ifndef SPATIAL_HASH_GRID_HH
#define SPATIAL_HASH_GRID_HH

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace SHG {
  template <typename T = std::nullptr_t>
  struct Entity {
    std::uint32_t x, y;

    Entity<T> *next = nullptr;
    Entity<T> *prev = nullptr;

    T data;
  };

  template <typename T = std::nullptr_t>
  class SpatialHashGrid {
    private:
      inline std::uint32_t pos_to_cell_index(std::uint32_t x, std::uint32_t y) noexcept {
        return (y >> this->cell_size_log2) * this->grid_width_log2 + (x >> this->cell_size_log2);
      }

    public:
      const std::uint8_t cell_size_log2, grid_width_log2, grid_height_log2;
      const std::uint32_t cell_size, cell_count, grid_width, grid_height;

      Entity<T> **grid;

      SpatialHashGrid(
        std::uint32_t cell_size, std::uint32_t grid_width, std::uint32_t grid_height) :
          cell_size_log2(31 - __builtin_clz(cell_size)),
          grid_width_log2(31 - __builtin_clz(grid_width)),
          grid_height_log2(31 - __builtin_clz(grid_height)),

          cell_size(cell_size),
          cell_count(grid_width * grid_height / (cell_size << 1)),

          grid_width(grid_width),
          grid_height(grid_height),
          
          grid(new Entity<T>*[cell_count]) {

        if (cell_size & (cell_size - 1))
          throw std::invalid_argument("Cell size must be a power of 2.");

        if ((grid_height & (grid_height - 1)) || (grid_width & (grid_width - 1)))
          throw std::invalid_argument("Grid dimensions must be powers of 2.");

        if (grid_height * grid_width < cell_size)
          throw std::invalid_argument("The area of the grid must be greater than the cell size.");

        std::fill(this->grid, this->grid + this->cell_count, nullptr);
      }

      ~SpatialHashGrid<T>() {
        delete[] this->grid;
      }

      inline void add(Entity<T> *entity) {
        auto &first_entity = this->grid[this->pos_to_cell_index(entity->x, entity->y)];

        if (first_entity) {
          entity->next = first_entity;
          first_entity->prev = entity;
        }

        first_entity = entity;
        entity->prev = nullptr;
      }

      inline void remove(Entity<T> *entity) {
        auto &first_entity = this->grid[this->pos_to_cell_index(entity->x, entity->y)];

        if (entity == first_entity) first_entity = first_entity->next;

        if (entity->prev != nullptr) entity->prev->next = entity->next;
        if (entity->next != nullptr) entity->next->prev = entity->prev;

        entity->prev = entity->next = nullptr;
      }

      inline void update() {
        for (std::uint32_t i = 0; i < this->cell_count; i++) {
          Entity<T> *current = this->grid[i];

          while (current) {
            if (i != this->pos_to_cell_index(current->x, current->y)) {
              this->remove(current);
              this->add(current);
            }

            current = current->next;
          }
        }
      }

      std::vector<Entity<T> *> query(
        std::uint32_t pos_x,
        std::uint32_t pos_y,
        std::uint32_t w,
        std::uint32_t h
      ) {
        std::vector<Entity<T> *> results;

        const std::uint32_t start_grid_x = (pos_x - w) >> this->cell_size_log2;
        const std::uint32_t start_grid_y = (pos_y - h) >> this->cell_size_log2;
        const std::uint32_t end_grid_x = (pos_x + w) >> this->cell_size_log2;
        const std::uint32_t end_grid_y = (pos_y + h) >> this->cell_size_log2;
        
        for (std::uint32_t grid_y = start_grid_y; grid_y <= end_grid_y; grid_y++) {
          for (std::uint32_t grid_x = start_grid_x; grid_x <= end_grid_x; grid_x++) {
            Entity<T> *current = this->grid[grid_y * this->grid_width_log2 + grid_x];

            while (current) {
              results.push_back(current);

              current = current->next;
            }
          }
        }

        return results;
      };
  };
} // namespace SHG

#endif
