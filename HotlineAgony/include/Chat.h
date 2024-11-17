#pragma once
#include <sstream>
#include "Player.h"

using game::living::Player;

namespace game::drawing::Gui {
	class Chat {
	public:
		static void Draw();
		static void Update(Player& sender);
		static void Enable();
		static void Disable();
		static void Send(Player& sender);
		static bool isEnabled();
		static std::stringstream chatInput;
	private:
		static bool m_chatEnabled;
	};
}