#include "Environment.h"
#include "PhysicsWorld.h"
#include "CollisionHandler.h"
#include "PhysicsDefs.h"
#include "FixtureDefBuilder.h"
#include "TexturePool.h"
#include <raymath.h>


using game::global::Environment;
using game::global::TexturePool;
using game::drawing::Tilemap;

Tilemap* Environment::m_tilemap = nullptr;
bool Environment::m_showHitboxes = false;
float Environment::tilemap_size_multiplier = 0.5f;

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

void Environment::DrawTilemap(const Camera2D& camera, const Vector2& position) {
	Tilemap& tilemap = *m_tilemap;
	TilemapBuffer_t& buffer = *tilemap.getBuffer();
	for (size_t height = 0; height < buffer.size(); ++height) {
		for (size_t column = 0; column < buffer.at(height).size(); ++column) {
			for (size_t row = 0; row < buffer.at(height).at(column).size(); ++row) {
				const Tile& tile = buffer.at(height).at(column).at(row);

				/* Unfinished system for drawing only tiles that camera can see - DO NOT DELETE!!!

				const Texture& tile_texture = TexturePool::GetTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::WALL));
				
				Vector2 newPos = Vector2Add(Vector2Divide(position, camera.offset), Vector2Add(Vector2Multiply(position, Vector2{camera.zoom, camera.zoom}), Vector2{(float)column * tile_texture.width / 2, (float)row * tile_texture.height / 2}));
				float camera_left = newPos.x - 100 / 2;
				float camera_right = newPos.x + 100 / 2;
				float camera_up = newPos.y - 100 / 2;
				float camera_bottom = newPos.y + 100 / 2;
				

				if (column * tile_texture.width >= camera_left &&
					column * tile_texture.width <= camera_right &&
					row * tile_texture.height >= camera_up &&
					row * tile_texture.height <= camera_bottom) {
					continue;
				}
				*/

				if (tile.texture == TileType::AIR) {
					continue;
				}
				const Texture2D& tex = TexturePool::GetTexture(TexturePoolTextureType::Building, static_cast<int>(tile.texture));
				float tile_size_x = tilemap.m_tile_width * tilemap_size_multiplier;
				float tile_size_y = tilemap.m_tile_height * tilemap_size_multiplier;
				Vector2 position = tilemap.getPosition();
				float tile_pos_x = (tile_size_x * column) - (ENV_TILES_COUNT * tile_size_x) / 2.0f + position.x;
				float tile_pos_y = (tile_size_y * row) - (ENV_TILES_COUNT * tile_size_y) / 2.0f + position.y;

				if (tile.texture == TileType::WALL) {
					tile_size_x *= 2;
					tile_size_y *= 2;
				}
				DrawTexturePro(tex, Rectangle{0, 0, tilemap.m_tile_width, tilemap.m_tile_height},
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
					Vector2 position = tilemap.getPosition();
					float tile_pos_x = (tile_size_x * column) - (ENV_TILES_COUNT * tile_size_x) / 2.0f + position.x;
					float tile_pos_y = (tile_size_y * row) - (ENV_TILES_COUNT * tile_size_y) / 2.0f + position.y;

					b2Body* wallbody;

					float tile_rotation_rad = DEG2RAD * static_cast<float>(tile.rotation);
					
					b2PolygonShape wallshape;
					wallshape.SetAsBox(8 * tilemap_size_multiplier, 32 * tilemap_size_multiplier);

					game::physics::FixtureDefBuilder wall_fixture;
					wall_fixture.setShape(&wallshape);
					wall_fixture.setDensity(1.f);
					wall_fixture.setFriction(0.3f);
					wall_fixture.setSensor(false);
					wall_fixture.setCategoryBits(COLLISION_WORLD);
					wall_fixture.setMaskBits(COLLISION_WORLD | COLLISION_PLR_HITBOX | COLLISION_WEAPON);
					wall_fixture.setPhysicsData(new PhysicsData(PhysicsBodyType::Wall, NULL));

					wallbody = PhysicsWorld::CreateBody(b2_staticBody,
						b2Vec2(tile_pos_x - 12 * cos(tile_rotation_rad), 
							tile_pos_y - 12 * sin(tile_rotation_rad)), 
						tile_rotation_rad, 0, 0, wall_fixture.getResult());
				}
			}
		}
	}
	PhysicsWorld::GetWorld().SetContactListener(new ContactListener);
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

void Environment::DrawTexture(Texture2D texture, Vector2 position, Vector2 origin, float width, float height, float rotation, float size) {
	/*
	DrawTexturePro(texture, Rectangle{ 0, 0, width, height },
		Rectangle{ position.x, position.y, width * size, height * size },
		Vector2{ width * size / (1 / origin.x), height * size / (1 / origin.y) },
		rotation, WHITE);*/
	DrawTexturePro(texture, Rectangle{ 0, 0, width, height },
		Rectangle{ position.x, position.y, width * size, height * size },
		Vector2{ origin.x, origin.y },
		rotation, WHITE);
}

void Environment::DrawTexture(Texture2D texture, Vector2 position, Vector2 origin, float width, float height, float rotation) {
	DrawTexturePro(texture, Rectangle{ 0, 0, width, height },
		Rectangle{ position.x, position.y, width, height },
		Vector2{ origin.x, origin.y },
		rotation, WHITE);
}

void Environment::DrawHitboxes() {
	PhysicsWorld::GetWorld().DebugDraw();
}

void Environment::DrawBullets() {
	b2Body* bodyList = PhysicsWorld::GetWorld().GetBodyList();

	while (bodyList->GetNext() != 0) {
		PhysicsData* data = reinterpret_cast<PhysicsData*>(bodyList->GetFixtureList()->GetUserData().pointer);
		
		if (data->body_type == PhysicsBodyType::Bullet && !data->isFlaggedToDelete) {
			DrawTexturePro(
				LoadTexture((std::string(GetApplicationDirectory()) + "/assets/effects/bullet.png").c_str()),
				Rectangle{ 0, 0, 15, 2 }, Rectangle{ bodyList->GetPosition().x, bodyList->GetPosition().y, 15, 2 }, Vector2{ 8, 1 }, bodyList->GetAngle() * RAD2DEG, WHITE);
		}
		bodyList = bodyList->GetNext();
	}
	
}

void Environment::DrawWeapons() {
	WeaponHandler::DrawWeapons();
}

inline void Environment::GameUpdate() {
	// TODO: write code here
}

void Environment::switchShowHitboxes() {
	m_showHitboxes = !m_showHitboxes;
}

bool Environment::isHitboxesShowing() {
	return m_showHitboxes;
}