# spatial-hash-grid
For 2D spaces and objects.

## Example
```cpp
#include <spatial_hash_grid.hh>

SHG::SpatialHashGrid shg(64, 1024, 1024);

std::vector<SHG::Entity<>> entities(1000, SHG::Entity<>());

// Add entities
for (auto &entity : entities) {
  shg.add(&entity);
}

// Query for nearby entities
for (std::uint32_t i = 0; i < entities.size(); i++) {
  shg.query(entities[i].x, entities[i].y, shg.cell_size, shg.cell_size);
}

// Update entity positions
shg.update();
```
