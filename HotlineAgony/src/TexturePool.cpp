#include "TexturePool.h"

using game::global::TexturePool;

std::unordered_map<std::pair<TexturePoolTextureType, int>, Texture2D, HashFunc> TexturePool::TextureBuffer;

void TexturePool::AddTexture(TexturePoolTextureType type, int id, Texture2D&& texture) {
	TextureBuffer.emplace(std::make_pair(type, id), std::move(texture));
}

Texture2D& TexturePool::GetTexture(TexturePoolTextureType type, int id) {
	return TextureBuffer.at(std::make_pair(type, id));
}