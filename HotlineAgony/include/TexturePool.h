#pragma once
#include <unordered_map>
#include "raylib.h"
#include "utils.h"

struct HashFunc {
	template<class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& v) const {
		return std::hash<T1>()(v.first) ^ std::hash<T2>()(v.second) << 1;
	}
};

namespace game {
	namespace global {
		/**
		* @class TexturePool
		* @brief This class is used to get Texture2D objects from the GPU memory.
		*/
		class TexturePool
		{
		public:
			static void AddTexture(TexturePoolTextureType type, int id, Texture2D&& texture);
			static Texture2D& GetTexture(TexturePoolTextureType type, int id);
		private:
			static std::unordered_map<std::pair<TexturePoolTextureType, int>, Texture2D, HashFunc> TextureBuffer;
		};


	}
}
