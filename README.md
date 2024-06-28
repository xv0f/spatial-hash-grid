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
shg.query(entities[0].x, entities[0].y, shg.cell_size, shg.cell_size);

// Update entity positions
shg.update();
```
## Notes
This [video](https://www.youtube.com/watch?v=sx4IIQL0x7c&t=358s) at does a good job explaining what spatial hash grids are (although the way this is implemented is slightly different).

Your choice of `cell_size`, `grid_width`, and `grid_height` affects the speed of the spatial hash grid, so choose wisely. Scaling the numbers shown in the example seems to work well.
