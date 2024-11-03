#pragma once
#include <memory>
#include "utils.h"
#include "Tilemap.h"
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
			static void DrawTilemap(const Camera2D& camera, const Vector2& position);
			static void FillTilemap(TileType building, TILEMAP_LAYER layer);
			static void FillTilemapLine(int begin_x, int begin_y, int end_x, int end_y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation);
			static void FillTilemapSquare(int begin_x, int begin_y, int end_x, int end_y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation);
			static void setTilemapTileTexture(int x, int y, TILEMAP_LAYER layer, TileType texture, TileRotation rotation);
			static void GameUpdate();
			static void DrawTexture(Texture2D texture, Vector2 position, Vector2 origin, float width, float height, float size, float rotation);
			static void DrawTexture(Texture2D texture, Vector2 position, Vector2 origin, float width, float height, float rotation);
			static void DrawHitboxes();
			static void InitTilemapHitboxes();
			static void switchShowHitboxes();
			static bool isHitboxesShowing();
		private:
			static Tilemap* m_tilemap;
			static float tilemap_size_multiplier;
			static float dayTime; ///< Current day time. 0 for the 0:00 and 1 for the 23:59
			static bool m_showHitboxes;
		};
	}
}
