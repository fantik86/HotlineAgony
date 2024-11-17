#include <raylib.h>
#include "Chat.h"

using game::drawing::Gui::Chat;

std::stringstream Chat::chatInput;
bool Chat::m_chatEnabled;

void Chat::Draw() {
	if (!m_chatEnabled) {
		return;
	}

	DrawRectanglePro(Rectangle{ 0,
		static_cast<float>(GetScreenHeight()),
		static_cast<float>(GetScreenWidth()), 40 }, Vector2{ 0, 40 },
		0, Color{ 50, 50, 50, 240 });
	DrawTextPro(GetFontDefault(), chatInput.str().c_str(), Vector2{ 0, static_cast<float>(GetScreenHeight()) }, Vector2{ 0, 28 }, 0, 28, 1, WHITE);
}

void Chat::Update(Player& sender) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        Disable();
    }

    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
        if (chatInput.str().length() > 0) {

            std::string str = chatInput.str();
            str.pop_back();
            chatInput.seekp(0, std::ios_base::end);
            chatInput.str(str);
        }
    }

    if (IsKeyPressed(KEY_ENTER) && isEnabled()) {
        Send(sender);
    }


    if (isEnabled()) {
        Draw();
        if (GetCharPressed() != NULL) {
            chatInput << static_cast<char>(GetKeyPressed());
        }
        return;
    }

    if (IsKeyPressed(KEY_T)) {
        Enable();
    }

    if (IsKeyPressed(KEY_SLASH)) {
        Enable();
        chatInput << "/";
    }
}

void Chat::Enable() {
	m_chatEnabled = true;
}

static void clearChatInput() {
	Chat::chatInput.str("");
	Chat::chatInput.clear();
}

void Chat::Disable() {
	m_chatEnabled = false;
	clearChatInput();
}



void Chat::Send(Player& sender) {
	if (chatInput.str() == "/TEST") {
		TraceLog(8, "Tested :D");
	}
	else {
		TraceLog(9, chatInput.str().c_str());
	}
	Disable();
}

bool Chat::isEnabled() {
	return m_chatEnabled;
}