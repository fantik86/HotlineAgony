#include "Tilemap.h"

using game::drawing::Tilemap;

Tilemap::Tilemap(Vector2 position, float size, float tile_width, float tile_height) :
	m_position(position), m_size(size), m_tile_width(tile_width),
	m_tile_height(tile_height), buffer(std::make_unique<TilemapBuffer_t>()) {}

