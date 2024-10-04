#include "Environment.h"
#include "TexturePool.h"
#include "box2d/box2d.h"

using game::global::Environment;
using game::global::TexturePool;
using game::drawing::Tilemap;

Tilemap* Environment::m_tilemap = nullptr;
bool Environment::debug_draw_edges = false;
std::unique_ptr<b2World> Environment::m_gamePhysicsWorld = std::make_unique<b2World>(b2Vec2_zero);

void Environment::SetTilemap(Tilemap& tilemap) {
	m_tilemap = &tilemap;
}

void Environment::FillTilemap(TileType building, TILEMAP_LAYER layer) {
	Tilemap& tilemap = *m_tilemap;
	TilemapBuffer_t& buffer = *tilemap.getBuffer();
	for (auto& buffer_layer : buffer.at(layer)) {
		for (auto& tile : buffer_layer) {
			tile.texture = building;
		}
	}
}

void Environment::DrawTilemap() {
	Tilemap& tilemap = *m_tilemap;
	TilemapBuffer_t& buffer = *tilemap.getBuffer();
	for (size_t height = 0; height < buffer.size(); height++) {
		for (size_t column = 0; column < buffer.at(height).size(); column++) {
			for (size_t row = 0; row < buffer.at(height).at(column).size(); row++) {
				const Tile& tile = buffer.at(height).at(column).at(row);

				if (tile.texture == TileType::AIR) {
					continue;
				}

				const Texture2D& tex = TexturePool::GetTexture(TexturePoolTextureType::Building, static_cast<int>(tile.texture));
				float tile_pos_x = (tilemap.m_tile_width * column) - (ENV_TILES_COUNT * tilemap.m_tile_width) / 2.0f + tilemap.m_position.x;
				float tile_pos_y = (tilemap.m_tile_height * row) - (ENV_TILES_COUNT * tilemap.m_tile_height) / 2.0f + tilemap.m_position.y;
				float tile_size_x = tilemap.m_tile_width;
				float tile_size_y = tilemap.m_tile_height;
				if (tile.texture == TileType::WALL) {
					tile_size_x *= 2;
					tile_size_y *= 2;
				}
				DrawTexturePro(tex, Rectangle{ 0, 0, tilemap.m_tile_width, tilemap.m_tile_height },
					Rectangle{ tile_pos_x, tile_pos_y, tile_size_x, tile_size_y },
					Vector2{ tilemap.m_tile_width / 2, tilemap.m_tile_height / 2 }, 
					static_cast<float>(tile.rotation), WHITE);
			}
		}
	}
}

void Environment::InitTilemapHitboxes() {
	Tilemap& tilemap = *m_tilemap;
	TilemapBuffer_t& buffer = *tilemap.getBuffer();
	for (size_t height = 0; height < buffer.size(); height++) {
		for (size_t column = 0; column < buffer.at(height).size(); column++) {
			for (size_t row = 0; row < buffer.at(height).at(column).size(); row++) {
				const Tile& tile = buffer.at(height).at(column).at(row);

				if (tile.texture == TileType::WALL) {
					float tile_pos_x = (tilemap.m_tile_width * column) - (ENV_TILES_COUNT * tilemap.m_tile_width) / 2.0f + tilemap.m_position.x;
					float tile_pos_y = (tilemap.m_tile_height * row) - (ENV_TILES_COUNT * tilemap.m_tile_height) / 2.0f + tilemap.m_position.y;

					b2BodyDef bodyWallDef;
					bodyWallDef.type = b2_staticBody;
					bodyWallDef.position.Set(tile_pos_x, tile_pos_y);
					b2Body* wallbody = Environment::GetPhysicsWorld().CreateBody(&bodyWallDef);
					wallbody->SetTransform(b2Vec2(tile_pos_x - 8, tile_pos_y + 16), 0);

					b2PolygonShape wallshape;
					wallshape.SetAsBox(8, 32);

					b2FixtureDef wallfixture;
					wallfixture.shape = &wallshape;
					wallfixture.density = 1.0f;
					wallfixture.friction = 0.3f;


					wallbody->CreateFixture(&wallfixture);
				}
			}
		}
	}
}

void Environment::setTilemapTileTexture(int x, int y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation) {
	Tilemap& tilemap = *m_tilemap;
	TilemapBuffer_t& buffer = *tilemap.getBuffer();
	Tile& tile = buffer.at(layer).at(ENV_TILES_COUNT / 2 + x).at(ENV_TILES_COUNT / 2 + y);
	tile.texture = texture;
	tile.rotation = rotation;

	
}

b2World& Environment::GetPhysicsWorld() {
	return *m_gamePhysicsWorld;
}

inline void Environment::GameUpdate() {
	// TODO: write code here
}