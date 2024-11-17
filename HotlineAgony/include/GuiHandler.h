#pragma once
#include "raylib.h"
#include <memory>
#include <vector>

class GuiElement {
public:
	virtual ~GuiElement() = default;
	virtual void Draw() = 0;
};

class GuiText : public GuiElement {
public:
	GuiText(Font font, const char* text, Vector2 position,
		float fontSize, float spacing, Color tint) : m_font(font), m_text(text),
		m_position(position), m_fontSize(fontSize), m_spacing(spacing), m_tint(tint) {};
	
	void Draw() override {
		DrawTextEx(m_font, m_text, m_position, m_fontSize, m_spacing, m_tint);
	}
private:
	Font m_font;
	const char* m_text;
	Vector2 m_position;
	float m_fontSize;
	float m_spacing;
	Color m_tint;
};

class GuiTexture : public GuiElement {
public:
	GuiTexture(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint) : 
		m_texture(texture), m_source(source), m_dest(dest), m_origin(origin), m_rotation(rotation), m_tint(tint) {};

	void Draw() override {
		DrawTexturePro(m_texture, m_source, m_dest, m_origin, m_rotation, m_tint);
	}
private:
	Texture2D m_texture;
	Rectangle m_source;
	Rectangle m_dest;
	Vector2 m_origin;
	float m_rotation;
	Color m_tint;
};

class GuiRectangle : public GuiElement {
public:
	GuiRectangle(Rectangle rec, Vector2 origin, float rotation,
		Color color) :
		m_rectangle(rec), m_origin(origin), m_rotation(rotation), m_color(color) {};

	void Draw() override {
		DrawRectanglePro(m_rectangle, m_origin, m_rotation, m_color);
	}
private:
	Rectangle m_rectangle;
	Vector2 m_origin;
	float m_rotation;
	Color m_color;
};


namespace game::drawing::Gui {
	class GuiHandler {
	public:
		static void addText(Font font, const char* text, Vector2 position,
			float fontSize, float spacing, Color tint);
		static void addTexture(Texture2D texture, Rectangle source,
			Rectangle dest, Vector2 origin, float rotation, Color tint);
		static void addRectangle(Rectangle rec, Vector2 origin, float rotation,
			Color color);
		static void drawGui();
	private:
		static std::vector<std::unique_ptr<GuiElement>> m_guiElements;
		static bool m_guiHidden;
	};
}