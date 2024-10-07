#include "Environment.h"
#include "TexturePool.h"
#include "box2d/box2d.h"

using game::global::Environment;
using game::global::TexturePool;
using game::drawing::Tilemap;

Tilemap* Environment::m_tilemap = nullptr;
bool Environment::debug_draw_edges = false;
float Environment::tilemap_size_multiplier = 0.5f;
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
				float tile_size_x = tilemap.m_tile_width * tilemap_size_multiplier;
				float tile_size_y = tilemap.m_tile_height * tilemap_size_multiplier;
				float tile_pos_x = (tile_size_x * column) - (ENV_TILES_COUNT * tile_size_x) / 2.0f + tilemap.m_position.x;
				float tile_pos_y = (tile_size_y * row) - (ENV_TILES_COUNT * tile_size_y) / 2.0f + tilemap.m_position.y;

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
					float tile_size_x = tilemap.m_tile_width * tilemap_size_multiplier;
					float tile_size_y = tilemap.m_tile_height * tilemap_size_multiplier;
					float tile_pos_x = (tile_size_x * column) - (ENV_TILES_COUNT * tile_size_x) / 2.0f + tilemap.m_position.x;
					float tile_pos_y = (tile_size_y * row) - (ENV_TILES_COUNT * tile_size_y) / 2.0f + tilemap.m_position.y;

					b2BodyDef bodyWallDef;
					bodyWallDef.type = b2_staticBody;
					bodyWallDef.position.Set(tile_pos_x, tile_pos_y);
					b2Body* wallbody = Environment::GetPhysicsWorld().CreateBody(&bodyWallDef);
					float tile_rotation_rad = DEG2RAD * static_cast<float>(tile.rotation);
					wallbody->SetTransform(b2Vec2(tile_pos_x - 12 * cos(tile_rotation_rad), tile_pos_y - 12 * sin(tile_rotation_rad)), tile_rotation_rad);

					b2PolygonShape wallshape;
					wallshape.SetAsBox(8 * tilemap_size_multiplier, 32 * tilemap_size_multiplier);

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
	int tile_pos_x = ENV_TILES_COUNT / 2 + x;
	int tile_pos_y = ENV_TILES_COUNT / 2 + y;

	if (layer == TILEMAP_LAYER_WALL) {
		tile_pos_x = ENV_TILES_COUNT / 2 + (x * 2);
		tile_pos_y = ENV_TILES_COUNT / 2 + (y * 2);
	}
	
	Tile& tile = buffer.at(layer).at(tile_pos_x).at(tile_pos_y);

	tile.texture = texture;
	tile.rotation = rotation;
	if (layer == TILEMAP_LAYER_FLOOR) {
		tile.rotation = TileRotation::LEFT;
	}
}

void Environment::FillTilemapLine(int begin_x, int begin_y, int end_x, int end_y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation) {
	int dx = end_x - begin_x;
	int dy = end_y - begin_y;
	int length = std::sqrt(dx * dx + dy * dy) + 1;
	for (int i = 0; i < length; i++) {
		int x = begin_x + static_cast<int>(static_cast<double>(i) / (length - 1) * dx);
		int y = begin_y + static_cast<int>(static_cast<double>(i) / (length - 1) * dy);
		Tilemap& tilemap = *m_tilemap;
		TilemapBuffer_t& buffer = *tilemap.getBuffer();
		int tile_pos_x = ENV_TILES_COUNT / 2 + x;
		int tile_pos_y = ENV_TILES_COUNT / 2 + y;
		if (layer == TILEMAP_LAYER_WALL) {
			tile_pos_x = ENV_TILES_COUNT / 2 + (x * 2);
			tile_pos_y = ENV_TILES_COUNT / 2 + (y * 2);
		}
		Tile& tile = buffer.at(layer).at(tile_pos_x).at(tile_pos_y);

		tile.texture = texture;
		tile.rotation = layer == TILEMAP_LAYER_FLOOR ? TileRotation::LEFT : rotation;
	}
}

void Environment::FillTilemapSquare(int begin_x, int begin_y, int end_x, int end_y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation) {
	if (begin_x > end_x) {
		std::swap(begin_x, end_x);
	}
	if (begin_y > end_y) {
		std::swap(begin_y, end_y);
	}

	for (int i = begin_x; i < end_x; i++) {
		for (int j = begin_y; j < end_y; j++) {
			Environment::setTilemapTileTexture(i, j, layer, texture, layer == TILEMAP_LAYER_FLOOR ? TileRotation::LEFT : rotation);
		}
	}
}

void Environment::DrawTexture(Texture2D texture, Vector2 position, Vector2 origin, float width, float height, float size, float rotation) {
	
	DrawTexturePro(texture, Rectangle{ 0, 0, width, height },
		Rectangle{ position.x, position.y, width * size, height * size },
		Vector2{ width * size / (1 / origin.x), height * size / (1 / origin.y) },
		rotation, WHITE);
}

b2World& Environment::GetPhysicsWorld() {
	return *m_gamePhysicsWorld;
}

void Environment::DrawHitboxes() {
	m_gamePhysicsWorld.get()->DebugDraw();
}

inline void Environment::GameUpdate() {
	// TODO: write code here
}