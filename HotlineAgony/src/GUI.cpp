#include "GUI.h"

using game::drawing::GUI;

bool GUI::m_guiHidden = false;
std::vector<std::unique_ptr<GuiElement>> GUI::m_guiElements = {};

void GUI::addText(Font font, const char* text, Vector2 position,
	float fontSize, float spacing, Color tint) {
	GUI::m_guiElements.push_back(std::make_unique<GuiText>(font, text, position, fontSize, spacing, tint));
}

void GUI::addTexture(Texture2D texture, Rectangle source,
	Rectangle dest, Vector2 origin, float rotation, Color tint) {
	GUI::m_guiElements.push_back(std::make_unique<GuiTexture>(texture, source, dest, origin, rotation, tint));
}

void GUI::drawGui() {
	for (const auto& element : GUI::m_guiElements) {
		element.get()->Draw();
	}
}