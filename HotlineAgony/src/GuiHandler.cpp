#include "GuiHandler.h"
#include "Chat.h"

using game::drawing::Gui::GuiHandler;

bool GuiHandler::m_guiHidden = false;
std::vector<std::unique_ptr<GuiElement>> GuiHandler::m_guiElements = {};

void GuiHandler::addText(Font font, const char* text, Vector2 position,
	float fontSize, float spacing, Color tint) {
	GuiHandler::m_guiElements.push_back(std::make_unique<GuiText>(font, text, position, fontSize, spacing, tint));
}

void GuiHandler::addTexture(Texture2D texture, Rectangle source,
	Rectangle dest, Vector2 origin, float rotation, Color tint) {
	GuiHandler::m_guiElements.push_back(std::make_unique<GuiTexture>(texture, source, dest, origin, rotation, tint));
}

void GuiHandler::addRectangle(Rectangle rec, Vector2 origin, float rotation,
	Color color) {
	GuiHandler::m_guiElements.push_back(std::make_unique<GuiRectangle>(rec, origin, rotation, color));
}

void GuiHandler::drawGui() {
	for (const auto& element : GuiHandler::m_guiElements) {
		element.get()->Draw();
	}
	
}