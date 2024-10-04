#pragma once
#include "utils.h"
#include "Tilemap.h"
#include <array>
#include <memory>
#include "box2d/box2d.h"

using game::drawing::Tilemap;

namespace game {
	namespace global {
		/**
		* @class Environment
		* @brief The global game world class.
		*/
		class Environment
		{
		public:
			static void SetTilemap(Tilemap& tilemap);
			static void DrawTilemap();
			static void FillTilemap(TileType building, TILEMAP_LAYER layer);
			static void setTilemapTileTexture(int x, int y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation);
			static void GameUpdate();
			static b2World& GetPhysicsWorld();
			static void InitTilemapHitboxes();
			static bool debug_draw_edges;
		private:
			static Tilemap* m_tilemap;
			static std::unique_ptr<b2World> m_gamePhysicsWorld;
			float dayTime = 0.0f; ///< Current day time. 0 for the 0:00 and 100 for the 23:59
		};
	}
}

