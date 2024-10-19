#pragma once
#include <memory>
#include "options.h"
#include "raylib.h"
#include "utils.h"

typedef std::array<std::array<std::array<Tile, ENV_TILES_COUNT>, ENV_TILES_COUNT>, 3> TilemapBuffer_t; ///< Stores all tiles of the environment

namespace game {
    namespace drawing {
        class Tilemap
        {
        public:
            Tilemap(Vector2 position, float size, float tile_width, float tile_height);
            void setPosition(Vector2 position);
            Vector2 getPosition() const;
            void setSize();
            float getSize() const;
            Vector2 getTilePosition(const Tile& tile);
            inline TilemapBuffer_t* getBuffer() {
                return buffer.get();
            }
            float m_size; ///< Size multiplier of entire tilemap
            float m_tile_width, m_tile_height; ///< Width and height of every tile in the tilemap
        private:
            std::unique_ptr<TilemapBuffer_t> buffer; ///< Stores info about all tiles
            Vector2 m_position;
        };
    }
}